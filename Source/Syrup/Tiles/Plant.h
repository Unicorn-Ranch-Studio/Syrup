// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Effects/TileEffectTrigger.h"

#include "CoreMinimal.h"
#include "Tile.h"
#include "Resources/ResourceFaucet.h"
#include "Resources/ResourceType.h"
#include "Resources/ResourceSink.h"
#include "Plant.generated.h"

class UResourceSink;
class UApplyField;
class UPreventTrashSpawn;

DECLARE_LOG_CATEGORY_EXTERN(LogPlant, Log, All);

/* \/ ====== \/ *\
|  \/ APlant \/  |
\* \/ ====== \/ */
/**
 * A plant on the grid that grows, can take damage, and creates a protection range.
 */
UCLASS(Abstract, HideCategories = ("ActorTick", "Tile", "Replication", "Rendering", "Collision", "Actor", "Input", "HLOD", "WorldPartition", "Cooking", "DataLayers"))
class SYRUP_API APlant : public ATile, public IResourceFaucet
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */
	APlant();

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
	 */
	UFUNCTION(BlueprintCallable, Category = "Plant|Health")
	void NotifyIncomingDamage(int Amount, ATile* Cause);

	/**
	 * Causes the visual effects of this plant taking damage
	 * 
	 * @param Amount - The number of damage points to that were caused to this plant.
	 * @param Cause - The tile that caused this damage.
	 */
	UFUNCTION(BlueprintImplementableEvent , Category = "Plant|Health")
	void OnIncomingDamageRecived(int Amount, ATile* Cause);
	
	/**
	 * Updates this plant to have the new amount of health.
	 * 
	 * @param NewHealth - The new health of this plant.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Plant|Health")
	void SetHealth(int NewHealth);
	void SetHealth_Implementation(int NewHealth);

	/**
	 * Gets the current health of this plant.
	 * 
	 * @return The number of damage points this plant can take before dying.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Health")
	FORCEINLINE int GetHealth() const { return Health; };

	/**
	 * Gets the amount of damage this plant has taken.
	 *
	 * @return The amount of damage this plant has taken.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Health")	
	FORCEINLINE int GetDamageTaken() const { return DamageTaken; };

	/**
	 * Causes the effects of this plants death.
	 */
	void Die();

protected:
	//The various properties of the health of this plant.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant|Health")
	UResourceSink* HealthResourceSink;
	
	//Whether or not this plant has died.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Plant|Health")
	bool bHasDied = false;

	//The tiles damaging this plant next turn and by how much.
	UPROPERTY(BlueprintReadOnly)
	TMap<ATile*, int> TilesToIncomingDamages;

private:
	//The current maximum health of this plant.
	UPROPERTY(VisibleInstanceOnly, Category = "Plant|Health")
	int Health = 0;

	//The damage taken by this plant.
	UPROPERTY(VisibleInstanceOnly, Category = "Plant|Health")
	int DamageTaken = 0;

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
	UFUNCTION(BlueprintCallable, Category = "Plant|Growth", Meta = (WorldContext = "WorldContextObject"))
	static bool SowPlant(UObject* WorldContextObject, UPARAM(Ref) int& EnergyReserve, TSubclassOf<APlant> PlantClass, FTransform Transform);
	static bool SowPlant(UObject* WorldContextObject, UPARAM(Ref) int& EnergyReserve, TSubclassOf<APlant> PlantClass, FGridTransform Transform);
	static bool SowPlant(UObject* WorldContextObject, TSubclassOf<APlant> PlantClass, FTransform Transform);
	static bool SowPlant(UObject* WorldContextObject, TSubclassOf<APlant> PlantClass, FGridTransform Transform);

	/**
	 * Gets cost to plant this plant type.
	 * 
	 * @return The amount of energy required to plant a plant of this type.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Growth")
	FORCEINLINE int GetPlantingCost() const { return PlantingCost; };

protected:

	//The amount of energy required to plant a plant of this type.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plant|Growth", Meta = (ClampMin = "0"))
	int PlantingCost = 1;

	//Whether or not this plant has finished being planted. **may be cause of bugs**
	UPROPERTY(BlueprintReadOnly, Category = "Plant|Growth")
	bool bIsFinishedPlanting = true;

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
	UFUNCTION(BlueprintCallable, Category = "Plant|Effect")
	void SetRange(const int NewRange);

	/**
	 * Gets the current range of this plant type's effects.
	 * 
	 * @return The scale applied to the shape of this plant type to get all effected locations of this plant type's effects.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Effect")
	FORCEINLINE int GetRange() const { return Range; };

protected:
	
	/**
	 * Activates the appropriate effects given the trigger.
	 * 
	 * @param TriggerType - The type of trigger that was activated.
	 * @param Triggerer - The tile that triggered this effect.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger);
	void ReceiveEffectTrigger_Implementation(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger);

	//The relation of how resources are allocated to range and how that is displayed.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant|Effect")
	UResourceSink* RangeResourceSink;

	//The scale applied to the shape of this plant type to get all effected locations of this plant type's effects.
	UPROPERTY(VisibleInstanceOnly, Category = "Plant|Effect", Meta = (ClampMin = "0"))
	int Range = 1;

private:

	/**
	 * Gets the locations where the effects of this plant will apply.
	 * 
	 * @return A set of all locations where the effects of this plant will apply.
	 */
	UFUNCTION()
	TSet<FIntPoint> GetEffectLocations() const;

	/* /\ Effect /\ *\
	\* ------------ */

	/* -------------- *\
	\* \/ Resource \/ */
public:
	
	/**
	 * Updates this plant to have the new amount of production.
	 * 
	 * @param NewHealth - The new production of this plant.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Plant|Resources")
	void SetProduction(int NewProduction);
	void SetProduction_Implementation(int NewProduction);
	
	/**
	 * Gets the number of resources supplied by this plant.
	 * 
	 * @return The number of resources supplied by this plant.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Resources")
	FORCEINLINE int GetProduction() const { return Production; };

	/**
	 * Gets the type of resource produced by this plant.
	 * 
	 * @return The type  of resource produced by this plant.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Resources")
	FORCEINLINE EResourceType GetProductionType() const { return ProductionType; };

	/**
	 * Gets all the resources supplied by this plant.
	 * 
	 * @return The resources supplied by this plant.
	 */
	virtual FORCEINLINE TArray<UResource*> GetProducedResources() const override { return ProducedResources; };

	/**
	 * Gets the grid locations that this sink takes up.
	 *
	 * @return The grid locations that this sink takes up
	 */
	UFUNCTION(BlueprintPure, Category = "Resources")
	virtual FORCEINLINE TSet<FIntPoint> GetAllocatableLocations() const override { return GetEffectLocations(); };

protected:

	//The relation of how resources are allocated to production and how that is displayed.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant|Resources")
	UResourceSink* ProductionResourceSink;

	//The number of resource this plant can produce. Should be an even number.
	UPROPERTY(VisibleInstanceOnly, Category = "Plant|Resources", Meta = (ClampMin = "0"))
	int Production = 2;

	//The type of resource produced by this.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Resources")
	EResourceType ProductionType = EResourceType::Any;

private:
	//The resources produced by this.
	UPROPERTY()
	TArray<UResource*> ProducedResources;
	
	//The resources allocated to this.
	UPROPERTY()
	TArray<UResource*> AllocatedResources;

	/* /\ Resource /\ *\
	\* -------------- */

	/* -------- *\
	\* \/ UI \/ */

public:
	
	/**
	 * Gets the name of this plant type.
	 * 
	 * @return The text to use when referring to a plant of this type.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|UI")
	FORCEINLINE FText GetDisplayName() const { return DisplayName; };

protected:

	//The text to use when referring to a plant of this type.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|UI")
	FText DisplayName = FText();

	/* /\ UI /\ *\
	\* -------- */
};
/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */