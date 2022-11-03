// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundPlane.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Syrup/Tiles/GridLibrary.h"


/**
 * Applies one layer of a field in a given area.
 *
 * @param FieldType - The type of the field. Fields of different types are independent.
 * @param Locations - The locations to add the field in.
 * 
 * @return If this plane was in any of the effected locations.
 */
bool AGroundPlane::ApplyField(const EFieldType Type, const TSet<FIntPoint>& Locations)
{
	return AddFieldStrength(Type, 1, Locations);
}

/**
 * Removes one layer of field from a given area.
 *
 * @param FieldType - The type of the field. Fields of different types are independent.
 * @param Locations - The locations to remove the field from.
 * 
 * @return If this plane was in any of the effected locations.
 */
bool AGroundPlane::RemoveField(const EFieldType Type, const TSet<FIntPoint>& Locations)
{
	return AddFieldStrength(Type, -1, Locations);
}

/**
 * Creates and sets up the ground mesh.
 */
AGroundPlane::AGroundPlane()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root"));

	//Create ground plane
	GroundMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("GroundPlane"));
	GroundMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GroundMeshComponent->SetAbsolute(true, true, true);
}


/**
 * Sets the size of the ground mesh.
 * 
 * @param Transform - The new transform.
 */
void AGroundPlane::OnConstruction(const FTransform& Transform)
{
	GroundMeshComponent->SetStaticMesh(GroundMesh);
	GroundMeshComponent->SetMaterial(0, Material);
	GroundMeshComponent->CastShadow = false;
	GroundMeshComponent->NumCustomDataFloats = 2;
	GroundMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	GroundMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GroundMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	GroundMeshComponent->ClearInstances();
	GroundMeshComponent->InstancingRandomSeed = FMath::Rand();
	LocationsToInstanceIndices.Empty();
	FieldTypeToLocationToStrengths.Empty();

	for (int IndexX = -(PlaneSize.X / 2); IndexX < PlaneSize.X / 2 + PlaneSize.X % 2; IndexX++)
	{
		for (int IndexY = -(PlaneSize.Y / 2); IndexY < PlaneSize.Y / 2 + PlaneSize.Y % 2; IndexY++)
		{
			FIntPoint GridLocation = FIntPoint(IndexX, IndexY) + Offset;
			LocationsToInstanceIndices.Add(GridLocation, GroundMeshComponent->AddInstance(UGridLibrary::GridTransformToWorldTransform(FGridTransform(GridLocation)) * FTransform(FVector(0, 0, -0.1)), true));
		}
	}
}

/**
 * Edits a field's strength in the given area.
 *
 * @param FieldType - The type of the field. Fields of different types have independent strengths.
 * @param Strength - The value to add to the field strength. Note: Field values clamped to >= 0.
 * @param Locations - The locations to change the field strength in.
 * 
 * @return If this plane was in any of the effected locations.
 */
bool AGroundPlane::AddFieldStrength(const EFieldType FieldType, const int Strength, const TSet<FIntPoint>& Locations)
{
	bool ReturnValue = false;

	//Create field map if not it doesn't exist
	if (LocationsToInstanceIndices.IsEmpty())
	{
		OnConstruction(GetActorTransform());
	}
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
		ReturnValue = true;

		//Create new entry if needed
		if (!FieldTypeToLocationToStrengths.Find(FieldType)->Contains(EachLocation))
		{
			if (Strength > 0)
			{
				FieldTypeToLocationToStrengths.Find(FieldType)->Add(EachLocation, Strength);
				GroundMeshComponent->SetCustomDataValue(LocationsToInstanceIndices.FindRef(EachLocation), (uint8)FieldType, 1, true);
			}
		}
		else
		{
			//Change strength a location
			int NewValue = FieldTypeToLocationToStrengths.Find(FieldType)->FindRef(EachLocation);
			NewValue += Strength;
			if (NewValue <= 0)
			{
				//Eliminate value at location if field is too weak.
				FieldTypeToLocationToStrengths.Find(FieldType)->Remove(EachLocation);
				GroundMeshComponent->SetCustomDataValue(LocationsToInstanceIndices.FindRef(EachLocation), (uint8)FieldType, 0, true);
			}
			else
			{
				FieldTypeToLocationToStrengths.Find(FieldType)->Add(EachLocation, NewValue);
			}
		}
	}

	return ReturnValue;
}