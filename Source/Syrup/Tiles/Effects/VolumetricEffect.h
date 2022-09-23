// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEffect.h"
#include "VolumetricEffect.generated.h"

class AVolumetricEffectActor;

/**
 * A type of effect applied things entering the effect volume.
 */
UCLASS(Abstract)
class SYRUP_API UVolumetricEffect : public UTileEffect
{
	GENERATED_BODY()

public:

	/**
	 * Causes this effect.
	 *
	 * @param EffectedLocations - The locations to effect.
	 * @param EffectedTiles - The tiles to effect.
	 * @param EffectedNonTileLocations - The locations that are not covered by tiles to effect.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	virtual void Affect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile) override;

	/**
	 * Undoes the effect of this.
	 *
	 * @param EffectedLocations - The locations that were effected.
	 * @param EffectedTiles - The tiles that were effected.
	 * @param EffectedNonTileLocations - The locations that are not covered by tiles to that were effected.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	virtual void Unaffect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile) override;

protected:
	/**
	 * Applies the effect of this volume when overlap is begun.
	 * 
	 * @param OverlappedActor - The volume that was overlapped.
	 * @param OtherActor - The actor that entered the volume.
	 */
	UFUNCTION()
	virtual FORCEINLINE void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor) {};

	/**
	 * Applies the effect of this volume when overlap is ended.
	 * 
	 * @param OverlappedActor - The volume that was left.
	 * @param OtherActor - The actor that left the volume.
	 */
	UFUNCTION()
	virtual FORCEINLINE void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor) {};

	/**
	 * Gets the collision channels that this volume will overlap.
	 * 
	 * @return A bitwise integer specifying which channels will be overlapped.
	 */
	UFUNCTION()
	virtual FORCEINLINE int GetOverlappedChannels() { return 0; };

private:
	AVolumetricEffectActor* VolumeActor;
};
