// Fill out your copyright notice in the Description page of Project Settings.

#include "PreventTrashSpawn.h"

/* \/ ================== \/ *\
|  \/ UPreventTrashSpawn \/  |
\* \/ ================== \/ */

/**
 * Adds persistent to triggers.
 */
UPreventTrashSpawn::UPreventTrashSpawn()
{
	Triggers.Add(ETileEffectTriggerType::Persistent);
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