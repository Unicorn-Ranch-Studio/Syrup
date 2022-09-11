// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldsManager.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Tiles/GridLibrary.h"


AFieldsManager::AFieldsManager()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	//Get Tile Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Game/Tiles/SM_Tile.SM_Tile"));
	UStaticMesh* TileMesh = MeshRef.Object;
	check(TileMesh != nullptr);

	//Create subtile mesh
	GroundPlane = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("GroundPlane"));
	GroundPlane->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GroundPlane->SetAbsolute(true, true, true);
	GroundPlane->SetStaticMesh(TileMesh);
	GroundPlane->SetMaterial(0, GroundMaterial);
	GroundPlane->CastShadow = false;
	GroundPlane->NumCustomDataFloats = 2;
}

void AFieldsManager::OnConstruction(const FTransform& Transform)
{
	GroundPlane->ClearInstances();
	LocationsToInstanceIndices.Empty();

	for (int IndexX = -(FieldDomain.X / 2); IndexX < FieldDomain.X / 2 + FieldDomain.X % 2; IndexX++)
	{
		for (int IndexY = -(FieldDomain.Y / 2); IndexY < FieldDomain.Y / 2 + FieldDomain.Y % 2; IndexY++)
		{
			FIntPoint GridLocation = FIntPoint(IndexX, IndexY);
			LocationsToInstanceIndices.Add(GridLocation, GroundPlane->AddInstance(UGridLibrary::GridTransformToWorldTransform(FGridTransform(GridLocation)) * FTransform(FVector(0,0,-1)), true));
		}
	}
}

void AFieldsManager::BeginPlay()
{
	OnConstruction(GetActorTransform());
}

/**
 * Edits a field's strength in the given area.
 *
 * @param FieldType - The type of the field. Fields of differnt types have independent strengths.
 * @param Strength - The value to add to the field strength. Note: Field values clamped to >= 0.
 * @param Locations - The locations to change the feild strength in.
 */
void AFieldsManager::AddFieldStrength(const EFieldType FieldType, const int Strength, const TSet<FIntPoint> Locations)
{
	//Create field map if not it doesnt exist
	if (!FieldTypeToLocationToStrengths.Contains(FieldType))
	{
		FieldTypeToLocationToStrengths.Add(FieldType, TMap<FIntPoint, int>());
	}

	for (FIntPoint EachLocation : Locations)
	{
		//Skip if outside domain
		if (!LocationsToInstanceIndices.Contains(EachLocation))
		{
			continue;
		}

		//Create new entry if needed
		if (!FieldTypeToLocationToStrengths.Find(FieldType)->Contains(EachLocation))
		{
			if (Strength > 0)
			{
				FieldTypeToLocationToStrengths.Find(FieldType)->Add(EachLocation, Strength);
				UE_LOG(LogTemp, Warning, TEXT("Sucess ====  % s"), GroundPlane->SetCustomDataValue(LocationsToInstanceIndices.FindRef(EachLocation), (uint8)FieldType, 1, true) ? *FString("true") : *FString("false"));
			}
		}
		else
		{
			//Change strenth a location
			int NewValue = FieldTypeToLocationToStrengths.Find(FieldType)->FindRef(EachLocation);
			NewValue += Strength;
			if (NewValue <= 0)
			{
				//Eliminate value at location if field is too weak.
				FieldTypeToLocationToStrengths.Find(FieldType)->Remove(EachLocation);
				GroundPlane->SetCustomDataValue(LocationsToInstanceIndices.FindRef(EachLocation), (uint8)FieldType, 0, true);
			}
			else
			{
				FieldTypeToLocationToStrengths.Find(FieldType)->Add(EachLocation, NewValue);
			}
		}
	}

	//Mark Field Dirty
	FieldTypeToTilesToStrengths.Remove(FieldType);
}

/**
 * Queries a field's strength in the given location.
 *
 * @param Type - The type of the field.
 * @param Location - The locations to change the feild strength in.
 * @return The strength of the field at the given location. Field strength will be 0 if the field is not present.
 */
int AFieldsManager::GetFieldStrengthAtLocation(const EFieldType FieldType, const FIntPoint Location) const
{
	if (!FieldTypeToLocationToStrengths.Contains(FieldType) || !FieldTypeToLocationToStrengths.Find(FieldType)->Contains(Location))
	{
		return 0;
	}
	return FieldTypeToLocationToStrengths.Find(FieldType)->FindRef(Location);
}

/**
 * Gets the set of all tiles in a given field.
 *
 * @param Type - The type of the field to query.
 * @return A map of effected tiles and the maximum field strength that tile is in.
 */
TMap<ATile*, int> AFieldsManager::GetTilesInField(const EFieldType FieldType)
{
	//Load previous value if valid
	if (FieldTypeToTilesToStrengths.Contains(FieldType))
	{
		return FieldTypeToTilesToStrengths.FindRef(FieldType);
	}


	TMap<ATile*, int> ReturnValue = TMap<ATile*, int>();
	for (TPair<FIntPoint, int> EachLocationToStrength : FieldTypeToLocationToStrengths.FindRef(FieldType))
	{
		ATile* OverlappedTile;
		if (UGridLibrary::OverlapGridLocation(GetWorld(), EachLocationToStrength.Key, OverlappedTile, TArray<AActor*>()))
		{
			int NewStrength = EachLocationToStrength.Value;
			if (ReturnValue.Contains(OverlappedTile))
			{
				NewStrength = FMath::Max(NewStrength, ReturnValue.FindRef(OverlappedTile));
			}
			ReturnValue.Add(OverlappedTile, NewStrength);
		}
	}

	//Save field value for future refernce
	FieldTypeToTilesToStrengths.Add(FieldType, ReturnValue);
	return ReturnValue;
}