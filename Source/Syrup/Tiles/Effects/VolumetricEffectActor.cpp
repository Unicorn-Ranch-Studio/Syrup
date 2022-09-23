// Fill out your copyright notice in the Description page of Project Settings.


#include "VolumetricEffectActor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Syrup/Tiles/GridLibrary.h"

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
	RootComponent = (CollisionMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("Collision Mesh")));
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
void AVolumetricEffectActor::SetOverlapedChannels(int Channels)
{
	CollisionMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	for (int Channel = 0; Channels > 0; Channels = Channels << 1, Channel++)
	{
		if (Channels % 2)
		{
			CollisionMesh->SetCollisionResponseToChannel((ECollisionChannel)Channel, ECollisionResponse::ECR_Overlap);
		}
	}
}

/**
 * Adds tiles to this effect volume.
 *
 * @param TileLocations - The locations of the tiles to add to the volume.
 */
void AVolumetricEffectActor::AddTiles(TSet<FIntPoint> TileLocations)
{
	TArray<FTransform> InstanceTransfroms = TArray<FTransform>();
	for (FIntPoint EachTileLocation : TileLocations)
	{
		InstanceTransfroms.Add(UGridLibrary::GridTransformToWorldTransform(EachTileLocation));
	}
	CollisionMesh->AddInstances(InstanceTransfroms, false);
}