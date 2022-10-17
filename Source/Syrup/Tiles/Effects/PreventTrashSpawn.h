// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VolumetricEffect.h"
#include "PreventTrashSpawn.generated.h"

/* \/ ================== \/ *\
|  \/ UPreventTrashSpawn \/  |
\* \/ ================== \/ */
/**
 * Prevents trash from spawning in the effect volume.
 */
UCLASS(ClassGroup = (TileEffects), Meta = (BlueprintSpawnableComponent))
class SYRUP_API UPreventTrashSpawn : public UVolumetricEffect
{
	GENERATED_BODY()

public:
	/**
	 * Adds persistent to triggers.
	 */
	UPreventTrashSpawn();

protected:

	/**
	 * Gets the collision channels that this volume will overlap.
	 * 
	 * @return A bitwise integer specifying which channels will be overlapped.
	 */
	virtual TSet<TEnumAsByte<ECollisionChannel>> GetBlockedChannels() const override;
};
/* /\ ================== /\ *\
|  /\ UPreventTrashSpawn /\  |
\* /\ ================== /\ */