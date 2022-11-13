// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEffect.h"
#include "VolumetricEffect.generated.h"

class AVolumetricEffectActor;

/* \/ ================= \/ *\
|  \/ UVolumetricEffect \/  |
\* \/ ================= \/ */
/**
 * A type of effect applied things entering the effect volume.
 */
UCLASS(Abstract)
class SYRUP_API UVolumetricEffect : public UTileEffect
{
	GENERATED_BODY()
public:

	/*
	 * Causes this effect.
	 *
	 * @param Locations - The locations to effect.
	 */
	virtual void Affect(const TSet<FIntPoint>& Locations) override;

	/*
	 * Undoes this effect.
	 */
	virtual void Unaffect(const TSet<FIntPoint>& Locations) override;

protected:

	/**
	 * Applies the effect of this volume when overlap is begun.
	 * 
	 * @param OtherActor - The actor that entered the volume.
	 */
	UFUNCTION()
	virtual FORCEINLINE void OnBeginOverlap(AActor* OtherActor) {};

	/**
	 * Applies the effect of this volume when overlap is ended.
	 * 
	 * @param OtherActor - The actor that left the volume.
	 */
	UFUNCTION()
	virtual FORCEINLINE void OnEndOverlap(AActor* OtherActor) {};

	/**
	 * Gets the collision channels that this volume will overlap.
	 * 
	 * @return A bitwise integer specifying which channels will be overlapped.
	 */
	UFUNCTION()
	virtual FORCEINLINE TSet<TEnumAsByte<ECollisionChannel>> GetOverlappedChannels() const { return TSet<TEnumAsByte<ECollisionChannel>>(); };

	/**
	 * Gets the collision channels that this volume will block.
	 * 
	 * @return A bitwise integer specifying which channels will be block.
	 */
	UFUNCTION()
	virtual FORCEINLINE TSet<TEnumAsByte<ECollisionChannel>> GetBlockedChannels() const { return TSet<TEnumAsByte<ECollisionChannel>>(); };

private:
	/**
	 * Applies the effect of this volume when overlap is begun.
	 *
	 * @param OverlappedActor - The volume that was overlapped.
	 * @param OtherActor - The actor that entered the volume.
	 */
	UFUNCTION()
	void ReceiveBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	/**
	 * Applies the effect of this volume when overlap is ended.
	 *
	 * @param OverlappedActor - The volume that was left.
	 * @param OtherActor - The actor that left the volume.
	 */
	UFUNCTION()
	void ReceiveEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	//All actors currently being overlapped
	UPROPERTY()
	TSet<AActor*> OverlappedActors = TSet<AActor*>();

	//The actor that handles the collision of this.
	UPROPERTY()
	AVolumetricEffectActor* VolumeActor;
};
/* /\ ================= /\ *\
|  /\ UVolumetricEffect /\  |
\* /\ ================= /\ */
