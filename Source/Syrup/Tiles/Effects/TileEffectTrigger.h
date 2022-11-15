// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEffectTrigger.generated.h"

class ATile;

/* \/ ================== \/ *\
|  \/ ETileEffectTrigger \/  |
\* \/ ================== \/ */
/**
 * A single effect that a tile effector can have.
 */
UENUM(BlueprintType)
enum class ETileEffectTriggerType : uint8
{
	// \/ Phase triggers, Called sequentially after the player ends their turn. None of these will pass location or triggerer info. \/ //

	//This starts the phase during which all non-player actions will occur.
	NonPlayerTurn	UMETA(DisplayName = "Phase: Non-Player Turn"),
	//This starts the phase during which plants will activate any effects they have.
	PlantActive		UMETA(DisplayName = "Phase: Plant Active"),
	//This starts the phase during which trash causes damage.
	TrashDamage		UMETA(DisplayName = "Phase: Plant Damage"),
	//This starts the phase during which trash activates effects other than damage.
	TrashActive		UMETA(DisplayName = "Phase: Trash Active"),
	//This starts the phase during which trash will fall.
	TrashSpawn		UMETA(DisplayName = "Phase: Trashfall"),
	//This starts the phase during which plants grow.
	PlantsGrow		UMETA(DisplayName = "Phase: Plant Growing"),
	//This starts the phase during which all player actions will occur.
	PlayerTurn		UMETA(DisplayName = "Phase: Player Turn"),

	// /\ Phase triggers, Called sequentially after the player ends their turn. None of these will pass location or triggerer info. /\ //



	// \/ Local Triggers, Called by the affecter to notify when its state has changed. Will not pass triggerer info. \/ //
	
	//This effect will be triggered when affecter is activated and will first be able to start causing effects.
	OnActivated		UMETA(DisplayName = "Local Trigger: On Affecter Activated"),
	//This effect will be triggered when affecter is deactivated and will no longer be able to cause effects.
	OnDeactivated	UMETA(DisplayName = "Local Trigger: On Affecter Deactivated"),

	// /\ Local Triggers, Called by the affecter to notify when its state has changed. Will not pass triggerer info. /\ //



	// \/ Global Triggers, Called by game object to notify changes in the board state. All of these will pass location and triggerer info. \/ //

	//This effect will be triggered whenever a plant is planted and will pass the locations of that plant.
	PlantSpawned	UMETA(DisplayName = "Global Trigger: On Plant Spawned"),
	//This effect will be triggered whenever a plant is by trash and will pass the locations of that plant.
	PlantKilled		UMETA(DisplayName = "Global Trigger: On Plant Killed"),
	//This effect will be triggered whenever a trash is spawned and will pass the locations of that trash.
	TrashSpawned	UMETA(DisplayName = "Global Trigger: On Trash Spawned"),
	//This effect will be triggered whenever a trash is picked up by the player and will pass the locations of that trash.
	TrashPickedUp	UMETA(DisplayName = "Global Trigger: On Trash Picked Up")

	// /\ Global Triggers, Called by game object to notify changes in the board state. All of these will pass location and triggerer info. /\ //
};

//Creates an iterator for this enum. Make sure the 2nd value == the last enum value.
ENUM_RANGE_BY_COUNT(ETileEffectTriggerType, ETileEffectTriggerType::TrashPickedUp)

//The last phase effect trigger.
#define LAST_PHASE_TRIGGER		ETileEffectTriggerType::PlayerTurn
/* /\ ================== /\ *\
|  /\ ETileEffectTrigger /\  |
\* /\ ================== /\ */

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTileEffectTrigger, const ETileEffectTriggerType, TriggerType, const ATile*, Triggerer, const TSet<FIntPoint>&, Locations);