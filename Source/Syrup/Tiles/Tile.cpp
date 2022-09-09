// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "DrawDebugHelpers.h"

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
		FTransform TileWorldTransform = UGridLibrary::GridLocationToWorldTransform(UGridLibrary::PointLocationInDirection(Orientation, EachTileLocation) + GridLocation);
		TileWorldTransforms.Add(TileWorldTransform);
		
		FVector TileWorldLocation = TileWorldTransform.GetTranslation();
		FHitResult HitResult = FHitResult();

		checkCode
		(
			if (GetWorld()->LineTraceSingleByObjectType(HitResult, TileWorldLocation, TileWorldLocation + FVector(0, 0, -KINDA_SMALL_NUMBER), FCollisionObjectQueryParams::AllDynamicObjects))
			{
				UE_LOG(LogTemp, Warning, TEXT("Tiles overlaping at: %s"), *TileWorldLocation.ToString());
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