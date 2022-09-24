// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VolumetricEffectActor.generated.h"


/* \/ ====================== \/ *\
|  \/ AVolumetricEffectActor \/  |
\* \/ ====================== \/ */
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
	void SetOverlapedChannels(TSet<TEnumAsByte<ECollisionChannel>> Channels);

	/**
	 * Adds tiles to this effect volume.
	 * 
	 * @param TileLocations - The locations of the tiles to add to the volume.
	 */
	UFUNCTION(BlueprintCallable)
	void AddTiles(TSet<FIntPoint> TileLocations);

	/**
	 * Removes tiles from this effect volume.
	 * 
	 * @param TileLocations - The locations of the tiles to remove from the volume.
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveTiles(TSet<FIntPoint> TileLocations);

//private:
	//The mesh used to generate overlap events.
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* CollisionMesh;

	//The instances used to represent each location.
	UPROPERTY()
	TMap<FIntPoint, int> InstanceLocationsToIndices = TMap<FIntPoint, int>();
};
/* /\ ====================== /\ *\
|  /\ AVolumetricEffectActor /\  |
\* /\ ====================== /\ */
