// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/ArrowComponent.h"
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
	UArrowComponent* Arrow = CreateDefaultSubobject<UArrowComponent>(FName("World Location"));
	Arrow->ArrowSize = 0.4;
	Arrow->ArrowLength = 50;
	Arrow->ArrowColor = FColor::Black;
	Arrow->SetHiddenInGame(true);
	RootComponent = Arrow;

	//Get Tile Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Game/Tiles/SM_Tile.SM_Tile"));
	TileMesh = MeshRef.Object;
	check(TileMesh != nullptr);

	//Create sub-tile mesh
	SubtileMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("Subtile Meshes"));
	SubtileMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SubtileMesh->SetAbsolute(true, true);
	SubtileMesh->SetStaticMesh(TileMesh);
	SubtileMesh->SetMaterial(0, TileMaterial);
	SubtileMesh->NumCustomDataFloats = 3;
	SubtileMesh->CastShadow = false;

	//Create Label Root
	LabelRoot = CreateDefaultSubobject<USceneComponent>(FName("Label Root"));
	LabelRoot->AttachToComponent(SubtileMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

/**
 * Gets the grid transform this tile.
 *
 * @return The grid transform this tile.
 */
void ATile::OnConstruction(const FTransform& Transform)
{
	SetActorTransform(Transform * (FTransform(-FVector(0, 0, Transform.GetTranslation().Z))));
	FieldsToStrengths = TMap<EFieldType, int>();
	if(ensure(IsValid(SubtileMesh)))
	{
		SubtileMesh->SetMaterial(0, TileMaterial);

		FGridTransform GridTransform = GetGridTransform();


		//Reset Mesh
		SubtileMesh->ClearInstances();
		SubtileMesh->InstancingRandomSeed = FMath::Rand() + 1;
		SubtileMesh->SetWorldTransform(UGridLibrary::GridTransformToWorldTransform(GridTransform));

		//Ensure tile has valid origin
		TSet<FIntPoint> TileLocations = GetRelativeSubTileLocations();
		TileLocations.Add(FIntPoint::ZeroValue);

		//Get sub-tile transforms
		TArray<FTransform> TileLocalTransforms = TArray<FTransform>();
		for (FIntPoint EachTileLocation : TileLocations)
		{
			TileLocalTransforms.Add(UGridLibrary::GridTransformToWorldTransform(FGridTransform(EachTileLocation)) * FTransform(FVector(UGridLibrary::GetGridHeight() * -0.333333333333333,0,0)));
			checkCode
			(
				ATile* OverlapedTile = nullptr;
				TArray<AActor*> IgnoredActors = TArray<AActor*>();
				IgnoredActors.Add(this);

				if (UGridLibrary::OverlapGridLocation(this, UGridLibrary::TransformGridLocation(EachTileLocation, GridTransform), OverlapedTile, IgnoredActors))
				{
					UE_LOG(LogLevel, Warning, TEXT("%s is overlapping %s at: %s"), *GetName(), *OverlapedTile->GetName(), *GridTransform.Location.ToString());
					DrawDebugPoint(GetWorld(), (TileLocalTransforms.Last() * SubtileMesh->GetComponentTransform()).GetLocation() + FVector(0, 0, 50), 50, FColor::Red, false, 5);
				}
			);
		}

		SubtileMesh->AddInstances(TileLocalTransforms, false, false);
	}
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

/**
 * Applies a field to this tile.
 *
 * @param Type - The type of field to apply.
 */
void ATile::ApplyField(EFieldType Type)
{
	if (FieldsToStrengths.Contains(Type))
	{
		FieldsToStrengths.Add(Type, FieldsToStrengths.FindRef(Type) + 1);
		return;
	}
	FieldsToStrengths.Add(Type,  1);
	UpdateField(Type, true);
	for (int InstanceIndex = 0; InstanceIndex < SubtileMesh->PerInstanceSMCustomData.Num(); InstanceIndex++)
	{
		SubtileMesh->SetCustomDataValue(InstanceIndex, (uint8)Type, 1, true);
	}
}

/**
 * Removes a field to this tile.
 *
 * @param Type - The type of field to remove.
 */
void ATile::RemoveField(EFieldType Type)
{
	if (FieldsToStrengths.Contains(Type))
	{
		int NewStrength = FieldsToStrengths.FindRef(Type) - 1;
		if (NewStrength > 0)
		{
			FieldsToStrengths.Add(Type, NewStrength);
		}
		else
		{
			FieldsToStrengths.Remove(Type);
			UpdateField(Type, false);
			for (int InstanceIndex = 0; InstanceIndex < SubtileMesh->PerInstanceSMCustomData.Num(); InstanceIndex++)
			{
				SubtileMesh->SetCustomDataValue(InstanceIndex, (uint8)Type, 0, true);
			}
		}
	}
}

/*
 * The relative locations of all of the sub-tiles of this tile.
 *
 * @return The relative locations of all of the sub-tiles of this tile.
 */
TSet<FIntPoint> ATile::GetRelativeSubTileLocations() const
{
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>();
	ReturnValue.Add(FIntPoint::ZeroValue);
	return ReturnValue;
}

/*
 * The locations of all of the sub-tiles of this tile.
 *
 * @return The locations of all of the sub-tiles of this tile.
 */
TSet<FIntPoint> ATile::GetSubTileLocations() const
{
	FGridTransform GridTransform = GetGridTransform();
	TSet<FIntPoint> ReturnValues = UGridLibrary::PointShapeInDirection(GridTransform.Direction, GetRelativeSubTileLocations());
	for (FIntPoint& EachReturnValue : ReturnValues)
	{
		EachReturnValue += GridTransform.Location;
	}

	return ReturnValues;
}
/* /\ ===== /\ *\
|  /\ ATile /\  |
\* /\ ===== /\ */