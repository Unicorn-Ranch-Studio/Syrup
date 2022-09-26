// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Effects/TileEffectTrigger.h"

#include "CoreMinimal.h"
#include "Tile.h"
#include "Plant.generated.h"

class UTileAffecterComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogPlant, Log, All);

/* \/ ====== \/ *\
|  \/ APlant \/  |
\* \/ ====== \/ */
/**
 * An object that snaps to the grid.
 *
 * May take up multiple grid locations by adding to the grid locations array.
 */
UCLASS()
class SYRUP_API APlant : public ATile
{
	GENERATED_BODY()

public:
	/**
	 * Creates the mesh component.
	 */
	APlant();

	/**
	 * Causes this plant to take damage.
	 * 
	 * @param Amount - The number of damage points to damage this plant by.
	 * @param Cause - The tile that caused this damage.
	 * 
	 * @return Whether or not this plant was killed by the damage.
	 */
	UFUNCTION(BlueprintCallable)
	bool ReceiveDamage(int Amount, ATile* Cause);

	/**
	 * Gets the current health of this plant.
	 * 
	 * @return The number of damage points this plant can take before dying.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetHealth() const { return Health; };

	/**
	 * Gets whether or not this plant is fully grown.
	 * 
	 * @return Whether or not this plant is fully grown.
	 */
	UFUNCTION(BlueprintPure)
	bool IsGrown() const;

	/**
	 * Gets the name of this plant type.
	 * 
	 * @return The text to use when referring to a plant of this type.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant")
	FORCEINLINE FText GetDisplayName() const { return DisplayName; };

	/**
	 * Gets the mesh of this plant type.
	 * 
	 * @return The static mesh to use for this plant type.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant")
	FORCEINLINE UStaticMesh* GetMesh() const { return Mesh; };

	/**
	 * Gets the shape of this plant type.
	 * 
	 * @return A set containing all of the relative locations of the sub-tiles making up the shape of this plant type.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant")
	FORCEINLINE TSet<FIntPoint> GetShape() const { return Shape; };

	/**
	 * Gets the max health of this plant type.
	 * 
	 * @return The number of damage points a plant of this type can take before dying.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant")
	FORCEINLINE int GetMaxHealth() const { return MaxHealth; };

	/**
	 * Gets the turns taken for this plant type to grow.
	 * 
	 * @return The number of turns it takes for this plant type's effects to begin taking effect.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant")
	FORCEINLINE int GetTimeUntilGrown() const { return TimeUntilGrown; };

	/**
	 * Gets cost to plant this plant type.
	 * 
	 * @return The amount of energy required to plant a plant of this type.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant")
	FORCEINLINE int GetPlantingCost() const { return PlantingCost; };

	/**
	 * Gets the range of this plant type's effects.
	 * 
	 * @return The scale applied to the shape of this plant type to get all effected locations of this plant type's effects.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant")
	FORCEINLINE int GetRange() const { return Range; };

protected:
	//The mesh of this plant.
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	//The health of this plant.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int Health = 1;

	//The turns remaining until this plant is fully grown.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int TimeUntilGrown = 1;

	//The text to use when referring to a plant of this type.
	UPROPERTY(EditDefaultsOnly)
	FText DisplayName = FText();

	//The static mesh to use for this plant type.
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* Mesh = nullptr;

	//A set containing all of the relative locations of the sub-tiles making up the shape of this plant type.
	UPROPERTY(EditDefaultsOnly)
	TSet<FIntPoint> Shape = TSet<FIntPoint>();

	//The number of damage points a plant of this type can take before dying.
	UPROPERTY(EditDefaultsOnly, Meta=(ClampMin = "1"))
	int MaxHealth = 1;

	//The number of turns it takes for this plant type's effects to begin taking effect.
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	int InitailTimeUntilGrown = 1;

	//The amount of energy required to plant a plant of this type.
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	int PlantingCost = 1;

	//The scale applied to the shape of this plant type to get all effected locations of this plant type's effects.
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	int Range = 1;

	/**
	 * The relative locations of all of the sub-tiles of this plant.
	 * 
	 * @return The relative locations of all of the sub-tiles of this plant.
	 */
	virtual TSet<FIntPoint> GetRelativeSubTileLocations() const override;

private:
	/**
	 * Binds effect triggers.
	 */
	virtual void BeginPlay() override;

	/**
	 * Initializes Health, Range, and TimeUntilGrown; and sets the appropriate mesh.
	 * 
	 * @param Transform - The new transform of the plant.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

	/**
	 * Activates the appropriate effects given the trigger.
	 * 
	 * @param TriggerType - The type of trigger that was activated.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
	 */
	UFUNCTION()
	void ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint> LocationsToTrigger);
private:

	/**
	 * Gets the locations where the effects of this plant will apply.
	 * 
	 * @return A set of all locations where the effects of this plant will apply.
	 */
	UFUNCTION()
	TSet<FIntPoint> GetEffectLocations() const;

	/**
	 * Updates the plants so that it is 1 turn closer to fully grown, and causes the effects of being fully grown if needed.
	 */
	UFUNCTION()
	void Grow();
};
/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */