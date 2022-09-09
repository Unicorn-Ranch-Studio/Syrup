// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "DrawDebugHelpers.h"

ATile::ATile()
{
	TileLocations.Add(FIntPoint::ZeroValue);

	//Create Root
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	
	//Get Tile Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Game/Tiles/SM_Tile.SM_Tile"));
	TileMesh = MeshRef.Object;
	check(TileMesh != nullptr);

	//Create subtile mesh
	SubtileMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("Subtile Mesh Instances"));
	SubtileMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SubtileMesh->SetAbsolute(true);
	SubtileMesh->SetStaticMesh(TileMesh);
	SubtileMesh->SetMaterial(0, TileMaterial);
	SubtileMesh->CastShadow = false;
}

void ATile::OnConstruction(const FTransform& Transform)
{
	SubtileMesh->SetMaterial(0, TileMaterial);

	FIntPoint GridLocation = GetGridLocation();

	//Snap direction
	if (UGridLibrary::IsDirectionValidAtLocation(Orientation, GridLocation))
	{
		Orientation = UGridLibrary::FlipDirection(Orientation);
	}

	//Reset Mesh
	SubtileMesh->ClearInstances();
	SubtileMesh->InstancingRandomSeed = FMath::Rand();

	//Ensure tile has valid orign
	TileLocations.Add(FIntPoint::ZeroValue);

	//Get subtile transforms
	TArray<FTransform> TileWorldTransforms = TArray<FTransform>();
	for (FIntPoint EachTileLocation : TileLocations)
	{
		FIntPoint RotatedGridLocation = UGridLibrary::PointLocationInDirection(Orientation, EachTileLocation);
		FTransform TileWorldTransform = UGridLibrary::GridLocationToWorldTransform(RotatedGridLocation + GridLocation);
		TileWorldTransforms.Add(TileWorldTransform);

		FVector TileWorldLocation = TileWorldTransform.GetTranslation();
		FHitResult HitResult = FHitResult();

		checkCode
		(
			ATile* OverlapedTile = nullptr;
			if (UGridLibrary::OverlapGridLocation(this, RotatedGridLocation + GridLocation, OverlapedTile, TArray<AActor*>()))
			{
				UE_LOG(LogLevel, Warning, TEXT("%s is overlaping %s at: %s"), *GetName(), *OverlapedTile->GetName(), *TileWorldLocation.ToString());
				DrawDebugPoint(GetWorld(), TileWorldTransforms.Last().GetTranslation() + FVector(0, 0, 1), 50, FColor::Red, false, 5);
			}
		);
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