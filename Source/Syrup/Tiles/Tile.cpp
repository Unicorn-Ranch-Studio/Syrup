// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "DrawDebugHelpers.h"



/* \/ ===== \/ *\
|  \/ ATile \/  |
\* \/ ===== \/ */
/**
 * Adjusts the sub-tile mesh location so that it is always snapped to the
 * grid location and orientation closest to its world transform.
 *
 * @param Transform - The new transform of the tile.
 */
ATile::ATile()
{
	//Create Root
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	
	//Get Tile Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Game/Tiles/SM_Tile.SM_Tile"));
	TileMesh = MeshRef.Object;
	check(TileMesh != nullptr);

	//Create sub-tile mesh
	SubtileMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("Subtile Mesh Instances"));
	SubtileMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SubtileMesh->SetAbsolute(true, true);
	SubtileMesh->SetStaticMesh(TileMesh);
	SubtileMesh->SetMaterial(0, TileMaterial);
	SubtileMesh->CastShadow = false;
}

/**
 * Gets the grid transform this tile.
 *
 * @return The grid transform this tile.
 */
void ATile::OnConstruction(const FTransform& Transform)
{
	SetActorTransform(Transform * (FTransform(-FVector(0, 0, Transform.GetTranslation().Z))));
	SubtileMesh->SetMaterial(0, TileMaterial);

	FGridTransform GridTransform = GetGridTransform();


	//Reset Mesh
	SubtileMesh->ClearInstances();
	SubtileMesh->InstancingRandomSeed = FMath::Rand();
	SubtileMesh->SetWorldTransform(UGridLibrary::GridTransformToWorldTransform(GridTransform));

	//Ensure tile has valid origin
	TSet<FIntPoint> TileLocations = GetTileLocations();
	TileLocations.Add(FIntPoint::ZeroValue);

	//Get sub-tile transforms
	TArray<FTransform> TileWorldTransforms = TArray<FTransform>();
	for (FIntPoint EachTileLocation : TileLocations)
	{
		FIntPoint RotatedGridLocation = UGridLibrary::PointLocationInDirection(GridTransform.Direction, EachTileLocation);
		FTransform TileWorldTransform = UGridLibrary::GridTransformToWorldTransform(FGridTransform(RotatedGridLocation + GridTransform.Location));
		TileWorldTransforms.Add(TileWorldTransform);

		FVector TileWorldLocation = TileWorldTransform.GetTranslation();
		FHitResult HitResult = FHitResult();

		checkCode
		(
			ATile* OverlapedTile = nullptr;
			if (UGridLibrary::OverlapGridLocation(this, RotatedGridLocation + GridTransform.Location, OverlapedTile, TArray<AActor*>()))
			{
				UE_LOG(LogLevel, Warning, TEXT("%s is overlapping %s at: %s"), *GetName(), *OverlapedTile->GetName(), *TileWorldLocation.ToString());
				DrawDebugPoint(GetWorld(), TileWorldTransforms.Last().GetTranslation() + FVector(0, 0, 1), 50, FColor::Red, false, 5);
			}
		);
	}

	SubtileMesh->AddInstances(TileWorldTransforms, false, true);
}

/**
 * Gets the grid transform this tile.
 *
 * @return The grid transform this tile.
 */
FGridTransform ATile::GetGridTransform() const
{
	return UGridLibrary::WorldTransformToGridTransform(GetActorTransform());
}

/*
 * The relative locations of all of the sub-tiles of this tile.
 *
 * @return The relative locations of all of the sub-tiles of this tile.
 */
TSet<FIntPoint> ATile::GetTileLocations() const
{
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>();
	ReturnValue.Add(FIntPoint::ZeroValue);
	return ReturnValue;
}
/* /\ ===== /\ *\
|  /\ ATile /\  |
\* /\ ===== /\ */