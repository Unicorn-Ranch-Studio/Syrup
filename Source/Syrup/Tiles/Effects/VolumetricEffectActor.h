// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VolumetricEffectActor.generated.h"

UCLASS()
class SYRUP_API AVolumetricEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * Creates the instanced static mesh used for collision
	 */
	AVolumetricEffectActor();
	
	/**
	 * Sets the channels that this volume will overlap.
	 * 
	 * @param Channels - A bitwise int specifying all the channels to enable.
	 */
	UFUNCTION()
	void SetOverlapedChannels(int Channels);

	/**
	 * Adds tiles to this effect volume.
	 * 
	 * @param TileLocations - The locations of the tiles to add to the volume.
	 */
	UFUNCTION(BlueprintCallable)
	void AddTiles(TSet<FIntPoint> TileLocations);

private:
	//The mesh used to generate overlap events.
	UPROPERTY()
	UInstancedStaticMeshComponent* CollisionMesh;
};
