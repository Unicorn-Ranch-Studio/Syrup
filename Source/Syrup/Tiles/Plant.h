// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Effects/TileEffectTrigger.h"

#include "CoreMinimal.h"
#include "Tile.h"
#include "Plant.generated.h"

class UApplyField;
class UPreventTrashSpawn;

DECLARE_LOG_CATEGORY_EXTERN(LogPlant, Log, All);

/* \/ ====== \/ *\
|  \/ APlant \/  |
\* \/ ====== \/ */
/**
 * A plant on the grid that grows, can take damage, and creates a protection radius.
 */
UCLASS(Abstract, HideCategories = ("ActorTick", "Tile", "Replication", "Rendering", "Collision", "Actor", "Input", "HLOD", "WorldPartition", "Cooking", "DataLayers"))
class SYRUP_API APlant : public ATile
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

private:

	/**
	 * Binds effect triggers and initializes size.
	 */
	virtual void BeginPlay() override;

	/**
	 * Initializes Health, Range, and Shape; and sets the appropriate mesh.
	 *
	 * @param Transform - The new transform of the plant.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ------------ *\
	\* \/ Health \/ */

public:

	/**
	 * Causes this plant to take damage.
	 * 
	 * @param Amount - The number of damage points to damage this plant by.
	 * @param Cause - The tile that caused this damage.
	 * 
	 * @return Whether or not this plant was killed by the damage.
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool ReceiveDamage(int Amount, ATile* Cause);

	/**
	 * Causes the visual effects of this plant taking damage
	 * 
	 * @param Amount - The number of damage points to that were caused to this plant.
	 * @param Cause - The tile that caused this damage.
	 * @param bShouldDestroy - Whether or not the plant should be destroyed once the VFX has finished.
	 */
	UFUNCTION(BlueprintNativeEvent , Category = "Health")
	void OnDamageRecived(int Amount, ATile* Cause, bool bShouldDestroy);
	FORCEINLINE void OnDamageRecived_Implementation(int Amount, ATile* Cause, bool bShouldDestroy) { if (bShouldDestroy) { Destroy(); } };

	/**
	 * Gets the current health of this plant.
	 * 
	 * @return The number of damage points this plant can take before dying.
	 */
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE int GetHealth() const { return Health; };

	/**
	 * Gets the max health of this plant type.
	 * 
	 * @return The number of damage points a plant of this type can take before dying.
	 */
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE int GetMaxHealth() const { return Cast<APlant>(GetClass()->GetDefaultObject())->Health; };

protected:

	//The health of this plant.
	UPROPERTY(EditDefaultsOnly, Category = "Health", Meta = (ClampMin = "1"))
	int Health = 1;

	/* /\ Health /\ *\
	\* ------------ */

	/* ------------ *\
	\* \/ Growth \/ */

public:
	/**
	 * Plants a plant with the given transform.
	 *
	 * @param WoldContextObject - Any object in the would to spawn the plant in.
	 * @param EnergyReserve - The variable attempt to subtract the planting cost from.
	 * @param PlantClass - The type of plant to plant.
	 * @param Transform - The location to spawn the plant at.
	 *
	 * @return Whether there was enough energy and space to plant the plant.
	 */
	UFUNCTION(BlueprintCallable, Category = "Growth", Meta = (WorldContext = "WorldContextObject"))
	static bool SowPlant(UObject* WorldContextObject, UPARAM(Ref) int& EnergyReserve, TSubclassOf<APlant> PlantClass, FTransform Transform);
	static bool SowPlant(UObject* WorldContextObject, UPARAM(Ref) int& EnergyReserve, TSubclassOf<APlant> PlantClass, FGridTransform Transform);
	static bool SowPlant(UObject* WorldContextObject, TSubclassOf<APlant> PlantClass, FTransform Transform);
	static bool SowPlant(UObject* WorldContextObject, TSubclassOf<APlant> PlantClass, FGridTransform Transform);

	/**
	 * Gets the turns taken for this plant type to grow.
	 * 
	 * @return The number of turns it takes for this plant type's effects to begin taking effect.
	 */
	UFUNCTION(BlueprintPure, Category = "Growth")
	FORCEINLINE int GetTimeUntilGrown() const { return TimeUntilGrown; };

	/**
	 * Gets the total number of turns that it takes for this plant type to grow.
	 * 
	 * @return The total number of turns that it takes for this plant type to grow.
	 */
	UFUNCTION(BlueprintPure, Category = "Growth")
	FORCEINLINE int GetInitialTimeUntilGrown() const { return Cast<APlant>(GetClass()->GetDefaultObject())->TimeUntilGrown; };

	/**
	 * Gets whether or not this plant is fully grown.
	 * 
	 * @return Whether or not this plant is fully grown.
	 */
	UFUNCTION(BlueprintPure, Category = "Growth")
	FORCEINLINE bool IsGrown() const { return TimeUntilGrown <= 0; };

	/**
	 * Gets cost to plant this plant type.
	 * 
	 * @return The amount of energy required to plant a plant of this type.
	 */
	UFUNCTION(BlueprintPure, Category = "Growth")
	FORCEINLINE int GetPlantingCost() const { return PlantingCost; };

protected:

	//The percent grown that this plant will start at.
	UPROPERTY(EditInstanceOnly, Category = "Growth", Meta = (ClampMin = "0", ClampMax = "1"))
	float InitialGrowthPercent = 0;

	//The turns remaining until this plant is fully grown.
	UPROPERTY(EditDefaultsOnly, Category = "Growth", Meta = (ClampMin = "1"))
	int TimeUntilGrown = 1;
	
	//The amount of energy required to plant a plant of this type.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Growth", Meta = (ClampMin = "0"))
	int PlantingCost = 1;

	//Whether or not this plant has finished being planted.
	UPROPERTY(BlueprintReadOnly, Category = "Growth")
	bool bIsFinishedPlanting = false;

	/**
	 * Updates the plants so that it is 1 turn closer to fully grown, and causes the effects of being fully grown if needed.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Grow();

	/**
	 * Updates the plants so that it is 1 turn closer to fully grown, and causes the effects of being fully grown if needed.
	 */
	UFUNCTION()
	void Grow_Implementation();

	/* /\ Growth /\ *\
	\* ------------ */

	/* ------------ *\
	\* \/ Effect \/ */

public:
	/**
	 * Sets the range of this plant's effects.
	 *
	 * @param NewRange - The value to set the range to. Will be clamped >= 0.
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void SetRange(const int NewRange);

	/**
	 * Gets the range of this plant type's effects.
	 * 
	 * @return The scale applied to the shape of this plant type to get all effected locations of this plant type's effects.
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE int GetRange() const { return Range; };

protected:

	//The scale applied to the shape of this plant type to get all effected locations of this plant type's effects.
	UPROPERTY(EditDefaultsOnly, Category = "Effect", Meta = (ClampMin = "0"))
	int Range = 1;

private:

	/**
	 * Activates the appropriate effects given the trigger.
	 * 
	 * @param TriggerType - The type of trigger that was activated.
	 * @param Triggerer - The tile that triggered this effect.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
	 */
	UFUNCTION()
	void ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger);

	/**
	 * Gets the locations where the effects of this plant will apply.
	 * 
	 * @return A set of all locations where the effects of this plant will apply.
	 */
	UFUNCTION()
	TSet<FIntPoint> GetEffectLocations() const;

	/* /\ Effect /\ *\
	\* ------------ */

	/* -------- *\
	\* \/ UI \/ */

public:
	
	/**
	 * Gets the name of this plant type.
	 * 
	 * @return The text to use when referring to a plant of this type.
	 */
	UFUNCTION(BlueprintPure, Category = "UI")
	FORCEINLINE FText GetDisplayName() const { return DisplayName; };

protected:

	//The text to use when referring to a plant of this type.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText DisplayName = FText();

	/* /\ UI /\ *\
	\* -------- */
};
/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */