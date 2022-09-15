// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "PlantData.h"
#include "Effects/TileEffectTrigger.h"

#include "CoreMinimal.h"
#include "Tile.h"
#include "Plant.generated.h"

class UPlantData;
class UTileAffecterComponent;

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

	//All of the information about this plants functionality.
	UPROPERTY(EditInstanceOnly)
	UPlantData* Data = nullptr;

	/**
	 * Causes this plant to take damage.
	 * 
	 * @param Amount - The number of damage points to damage this plant by.
	 * 
	 * @return Whether or not this plant was killed by the damage.
	 */
	UFUNCTION(BlueprintCallable)
	bool ReciveDamage(int Amount);

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

protected:
	//The mesh of this plant.
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* Mesh;

	//The health of this plant.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int Health = 1;

	//The range of all of the effects of this plant.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int Range = 1;

	//The turns remaining until this plant is fully grown.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int TimeUntilGrown = 1;

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
	void ReceiveEffectTrigger(const ETileEffectTrigger TriggerType, const TSet<FIntPoint> LocationsToTrigger);
	
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

	//The affecters linked to each effect trigger.
	UPROPERTY()
	TMap<ETileEffectTrigger, UTileAffecterComponent*> TriggersToAffectors = TMap<ETileEffectTrigger, UTileAffecterComponent*>();
};
/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */