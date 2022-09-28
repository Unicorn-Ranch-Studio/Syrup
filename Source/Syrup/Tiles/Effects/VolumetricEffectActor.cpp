// Fill out your copyright notice in the Description page of Project Settings.


#include "VolumetricEffectActor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Syrup/Tiles/GridLibrary.h"

/* \/ ====================== \/ *\
|  \/ AVolumetricEffectActor \/  |
\* \/ ====================== \/ */
/**
 * Creates the instanced static mesh used for collision
 */
AVolumetricEffectActor::AVolumetricEffectActor()
{
	//Get mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Game/Tiles/SM_Tile.SM_Tile"));
	UStaticMesh* VolumeMesh = MeshRef.Object;
	check(VolumeMesh != nullptr);

	//Get material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatRef(TEXT("/Game/Tiles/M_VolumetricEffectActor.M_VolumetricEffectActor"));
	UMaterialInterface* VolumeMat = MatRef.Object;
	check(VolumeMat != nullptr);

	//Create mesh component
	CollisionMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("Collision Mesh"));
	RootComponent = CollisionMesh;
	CollisionMesh->SetStaticMesh(VolumeMesh);
	CollisionMesh->SetMaterial(0, VolumeMat);
	CollisionMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	CollisionMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


/**
 * Sets the channels that this volume will overlap.
 *
 * @param Channels - A bitwise int specifying all the channels to enable.
 */
void AVolumetricEffectActor::SetCollisionResponses(const TSet<TEnumAsByte<ECollisionChannel>>& OverlapedChannels, const TSet<TEnumAsByte<ECollisionChannel>>& BlockedChannels)
{
	CollisionMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	for (ECollisionChannel EachOverlapedChannel : OverlapedChannels)
	{
		CollisionMesh->SetCollisionResponseToChannel(EachOverlapedChannel, ECollisionResponse::ECR_Overlap);
	}
	
	for (ECollisionChannel EachBlockedChannel : BlockedChannels)
	{
		CollisionMesh->SetCollisionResponseToChannel(EachBlockedChannel, ECollisionResponse::ECR_Block);
	}
}

/**
 * Adds tiles to this effect volume.
 *
 * @param TileLocations - The locations of the tiles to add to the volume.
 */
void AVolumetricEffectActor::AddTiles(const TSet<FIntPoint>& TileLocations)
{
	for (FIntPoint EachTileLocation : TileLocations)
	{
		InstanceLocationsToIndices.Add(EachTileLocation, CollisionMesh->AddInstance(UGridLibrary::GridTransformToWorldTransform(EachTileLocation)));
	}
}

/**
 * Removes tiles from this effect volume.
 *
 * @param TileLocations - The locations of the tiles to remove from the volume.
 */
void AVolumetricEffectActor::RemoveTiles(const TSet<FIntPoint>& TileLocations)
{
	for (FIntPoint EachTileLocation : TileLocations)
	{
		CollisionMesh->RemoveInstance(InstanceLocationsToIndices.FindRef(EachTileLocation));
	}
}
/* /\ ====================== /\ *\
|  /\ AVolumetricEffectActor /\  |
\* /\ ====================== /\ */