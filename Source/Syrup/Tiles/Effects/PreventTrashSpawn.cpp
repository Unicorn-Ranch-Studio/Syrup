// Fill out your copyright notice in the Description page of Project Settings.

#include "PreventTrashSpawn.h"

/* \/ ================== \/ *\
|  \/ UPreventTrashSpawn \/  |
\* \/ ================== \/ */

/*
 * Causes this effect.
 *
 * @param TriggerType - The type of effects that are currently being triggered.
 * @param Locations - The locations to effect.
 */
void UPreventTrashSpawn::Affect(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& Locations)
{
	if (TriggerType == ETileEffectTriggerType::Persistent)
	{
		Super::Affect(TriggerType, Locations);
	}
}

/*
 * Undoes this effect.
 *
 * @param TriggerType - The type of effects that are currently being undone.
 * @param Locations - The locations to undo the effect on.
 */
void UPreventTrashSpawn::Unaffect(const ETileEffectTriggerType TriggerType)
{
	if (TriggerType == ETileEffectTriggerType::Persistent)
	{
		Super::Unaffect(TriggerType);
	}
}

/**
 * Gets the collision channels that this volume will overlap.
 *
 * @return A bitwise integer specifying which channels will be overlapped.
 */
TSet<TEnumAsByte<ECollisionChannel>> UPreventTrashSpawn::GetBlockedChannels() const
{
	TSet<TEnumAsByte<ECollisionChannel>> ReturnValue = TSet<TEnumAsByte<ECollisionChannel>>();
	ReturnValue.Add(ECollisionChannel::ECC_GameTraceChannel2);
	return ReturnValue;
}
/* /\ ================== /\ *\
|  /\ UPreventTrashSpawn /\  |
\* /\ ================== /\ */