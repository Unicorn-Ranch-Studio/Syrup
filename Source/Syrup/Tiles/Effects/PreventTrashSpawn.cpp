// Fill out your copyright notice in the Description page of Project Settings.

#include "PreventTrashSpawn.h"

/* \/ ================== \/ *\
|  \/ UPreventTrashSpawn \/  |
\* \/ ================== \/ */
/**
 * Gets the collision channels that this volume will overlap.
 *
 * @return A bitwise integer specifying which channels will be overlapped.
 */
TSet<TEnumAsByte<ECollisionChannel>> UPreventTrashSpawn::GetOverlappedChannels() const
{
	TSet<TEnumAsByte<ECollisionChannel>> ReturnValue = TSet<TEnumAsByte<ECollisionChannel>>();
	ReturnValue.Add(ECollisionChannel::ECC_GameTraceChannel2);
	return ReturnValue;
}
/* /\ ================== /\ *\
|  /\ UPreventTrashSpawn /\  |
\* /\ ================== /\ */