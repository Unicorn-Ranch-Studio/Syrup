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
public:
	/**
	 * Creates the mesh component.
	 */
	APlant();

	//All of the information about this plants functionality.
	UPROPERTY(EditInstanceOnly)
	UPlantData* Data = nullptr;

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


private:
	/**
	 * Initializes Health, Range, and TimeUntilGrown; and sets the appropriate mesh.
	 */
	OnConstruct();

	/**
	 * Activates the appropriate effects given the trigger.
	 * 
	 * @param TriggerType - The type of trigger that was activated.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect.
	 */
	UFUNCTION()
	void ReceiveEffectTrigger(ETileEffectTrigger TriggerType, TSet<FIntPoints> LocationsToTrigger);

	//The affecters linked to each effect trigger.
	UPROPERTY()
	TMap<UTileAffecterComponent*> TriggersToAffectors = TMap<UTileAffecterComponent*>();
};
/* /\ ===== /\ *\
|  /\ ATile /\  |
\* /\ ===== /\ */