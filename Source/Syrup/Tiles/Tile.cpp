// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Components/InstancedStaticMeshComponent.h"

ATile::ATile()
{
	TileLocations.Add(FIntPoint::ZeroValue);

	//Get Tile Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Game/Tiles/SM_Tile.SM_Tile"));
	TileMesh = MeshRef.Object;
	check(TileMesh != nullptr);

	//Create subtile mesh
	SubtileMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("Subtile Mesh Instances"));
	SubtileMesh->SetStaticMesh(TileMesh);
	SubtileMesh->SetMaterial(0, TileMaterial);
	RootComponent = SubtileMesh;
}

void ATile::OnConstruction(const FTransform& Transform)
{
	SubtileMesh->SetMaterial(0, TileMaterial);

	FIntPoint GridLocation = GetGridLocation();

	SubtileMesh->ClearInstances();
	SubtileMesh->InstancingRandomSeed = FMath::Rand();

	//Snap direction
	if (UGridLibrary::IsDirectionValidAtLocation(Orientation, GridLocation))
	{
		Orientation = UGridLibrary::FlipDirection(Orientation);
	}

	//Ensure tile has valid orign
	TileLocations.Add(FIntPoint::ZeroValue);

	//Get subtile transforms
	TArray<FTransform> TileWorldTransforms = TArray<FTransform>();
	for (FIntPoint EachTileLocation : TileLocations)
	{
		TileWorldTransforms.Add(UGridLibrary::GridLocationToWorldTransform(UGridLibrary::PointLocationInDirection(Orientation, EachTileLocation) + GridLocation));
	}

	SubtileMesh->AddInstances(TileWorldTransforms, false, true);
}

/**
 * Gets the grid location this tile.
 *
 * @return The grid location this tile.
 */
FIntPoint ATile::GetGridLocation()
{
	return UGridLibrary::WorldLocationToGridLocation(GetActorLocation());
}