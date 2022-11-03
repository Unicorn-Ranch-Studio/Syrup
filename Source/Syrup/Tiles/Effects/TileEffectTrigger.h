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



	// \/ Local Triggers, Called by the affecter to notify when its state has changed. \/ //
	
	//This effect will be triggered when affecter is activated and will first be able to start causing effects.
	OnActivated		UMETA(DysplayName = "On Affecter Activated"),
	//This effect will be triggered when affecter is deactivated and will no longer be able to cause effects.
	OnDeactivated	UMETA(DysplayName = "On Affecter Deactivated"),

	// /\ Local Triggers, Called by the affecter to notify when its state has changed. /\ //



	// \/ Global Triggers, Called by game object to notify changes in the board state. All of these will pass location info. \/ //

	//This effect will be triggered whenever a plant is planted and will pass the locations of that plant.
	PlantSpawned	UMETA(DysplayName = "On Plant Spawned"),
	//This effect will be triggered whenever a plant is by trash and will pass the locations of that plant.
	PlantKilled		UMETA(DysplayName = "On Plant Killed"),
	//This effect will be triggered whenever a trash is spawned and will pass the locations of that trash.
	TrashSpawned	UMETA(DysplayName = "On Trash Spawned"),
	//This effect will be triggered whenever a trash is picked up by the player and will pass the locations of that trash.
	TrashPickedUp	UMETA(DysplayName = "On Trash Picked Up")

	// /\ Global Triggers, Called by game object to notify changes in the board state. All of these will pass location info. /\ //
};

//Creates an iterator for this enum. Make sure the 2nd value == the last enum value.
ENUM_RANGE_BY_COUNT(ETileEffectTriggerType, ETileEffectTriggerType::TrashPickedUp)

//The last phase effect trigger.
#define LAST_PHASE_TRIGGER		ETileEffectTriggerType::PlantsGrow
/* /\ ================== /\ *\
|  /\ ETileEffectTrigger /\  |
\* /\ ================== /\ */

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTileEffectTrigger, const ETileEffectTriggerType, TriggerType, const TSet<FIntPoint>&, Locations);