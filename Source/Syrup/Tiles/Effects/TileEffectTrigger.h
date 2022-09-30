// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEffectTrigger.generated.h"

/* \/ ================== \/ *\
|  \/ ETileEffectTrigger \/  |
\* \/ ================== \/ */
/**
 * A single effect that a tile effector can have.
 */
UENUM(BlueprintType)
enum class ETileEffectTriggerType : uint8
{
	//This effect will be triggered when affecter is activated and when other tiles are placed near the affecter after its activation.
	Persistent		UMETA(DysplayName = "Persistent"),



	// \/ Phase triggers, Called sequentially after the player ends their turn. None of these will pass location info. \/ //

	//This effect will be triggered right after the player ends their turn.
	PlantActive		UMETA(DysplayName = "Plant Active Phase"),
	//This effect will be triggered after the player ends their turn& plants activate their effects.
	TrashDamage		UMETA(DysplayName = "Plant Damage Phase"),
	//This effect will be triggered after the trash deals damage.
	TrashActive		UMETA(DysplayName = "Trash Active Phase"),
	//This effect will be triggered right after the player ends their turn.
	TrashSpread		UMETA(DysplayName = "Trash Spread Phase"),
	//This effect will be triggered right before the player starts their turn.
	PlantsGrow		UMETA(DysplayName = "Plant Growing Phase"),

	// /\ Phase triggers, Called sequentially after the player ends their turn. None of these will pass location info. /\ //



	// \/ Event Triggers, Called by game object to notify changes in the board state. All of these will pass location info. \/ //

	//This effect will be triggered whenever a trash is spawned and will pass the locations of that trash.
	TrashSpawned	UMETA(DysplayName = "On Trash Spawned"),
	//This effect will be triggered whenever a trash is picked up by the player and will pass the locations of that trash.
	TrashPickedUp	UMETA(DysplayName = "On Trash Picked Up")

	// /\ Event Triggers, Called by game object to notify changes in the board state. All of these will pass location info. /\ //
};

//Creates an iterator for this enum. Make sure the 2nd value == the last enum value.
ENUM_RANGE_BY_COUNT(ETileEffectTriggerType, ETileEffectTriggerType::PlantsGrow)
/* /\ ================== /\ *\
|  /\ ETileEffectTrigger /\  |
\* /\ ================== /\ */

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTileEffecTrigger, const ETileEffectTriggerType, TriggerType, const TSet<FIntPoint>&, Locations);