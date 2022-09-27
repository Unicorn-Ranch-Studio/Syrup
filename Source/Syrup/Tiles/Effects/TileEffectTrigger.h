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
	Persistent		UMETA(DysplayName = "Persistent", ToolTip = "This effect will be triggered when affecter is activated and when other tiles are placed near the affecter after its activation."),
	PlantActive		UMETA(DysplayName = "Plant Active Phase", ToolTip = "This effect will be triggered right after the player ends their turn."),
	TrashDamage		UMETA(DysplayName = "Plant Damage Phase", ToolTip = "This effect will be triggered after the player ends their turn & plants activate their effects."),
	TrashActive		UMETA(DysplayName = "Trash Active Phase", ToolTip = "This effect will be triggered after the trash deals damage."),
	TrashSpread		UMETA(DysplayName = "Trash Spread Phase", ToolTip = "This effect will be triggered right after the player ends their turn."),
	PlantsGrow		UMETA(DysplayName = "Plant Growing Phase", ToolTip = "This effect will be triggered right before the player starts their turn.")
};

//Creates an iterator for this enum. Make sure the 2nd value == the last enum value.
ENUM_RANGE_BY_COUNT(ETileEffectTriggerType, ETileEffectTriggerType::PlantsGrow)
/* /\ ================== /\ *\
|  /\ ETileEffectTrigger /\  |
\* /\ ================== /\ */

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTileEffecTrigger, ETileEffectTriggerType, TriggerType, TSet<FIntPoint>, Locations);