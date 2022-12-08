// Fill out your copyright notice in the Description page of Project Settings.

#include "PreventPlantSpawn.h"

/* \/ ================== \/ *\
|  \/ UPreventPlantSpawn \/  |
\* \/ ================== \/ */

/**
 * Adds persistent to triggers.
 */
UPreventPlantSpawn::UPreventPlantSpawn()
{
	AffectTriggers.Add(ETileEffectTriggerType::OnActivated);
	UnaffectTriggers.Add(ETileEffectTriggerType::OnDeactivated);
}

/**
 * Gets the collision channels that this volume will overlap.
 *
 * @return A bitwise integer specifying which channels will be overlapped.
 */
TSet<TEnumAsByte<ECollisionChannel>> UPreventPlantSpawn::GetBlockedChannels() const
{
	TSet<TEnumAsByte<ECollisionChannel>> ReturnValue = TSet<TEnumAsByte<ECollisionChannel>>();
	ReturnValue.Add(ECollisionChannel::ECC_GameTraceChannel3);
	return ReturnValue;
}
/* /\ ================== /\ *\
|  /\ UPreventPlantSpawn /\  |
\* /\ ================== /\ */