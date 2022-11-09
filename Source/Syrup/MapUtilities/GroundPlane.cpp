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



	const double ActorAngle = FMath::Fmod(GetActorRotation().Yaw, 360);
	const FVector2D PlaneSize = FVector2D(UGridLibrary::GetGridSideLength() * 25 * GetActorScale());
	FVector2D TopCornerLocation;
	FVector2D MiddleCornerLocation;
	FVector2D RowOffset;

	UE_LOG(LogTemp, Warning, TEXT("Mod: %f"), FMath::Fmod(ActorAngle + FMath::Sign(ActorAngle) * 0.01, 90))
	if (FMath::IsNearlyZero(FMath::Fmod(ActorAngle + FMath::Sign(ActorAngle) * 0.01, 90), 0.02))
	{
		FVector2D BoundsSize = ((FMath::IsNearlyZero(FMath::Fmod(ActorAngle + FMath::Sign(ActorAngle) * 0.01, 180), 0.02) ? PlaneSize : FVector2D(PlaneSize.Y, PlaneSize.X))* 4 / UGridLibrary::GetGridSideLength()) * FVector2D(0.57735027, 1);
		for (int IndexX = -(BoundsSize.X / 2); IndexX < BoundsSize.X / 2 + (int)BoundsSize.X % 2; IndexX++)
		{
			for (int IndexY = -(BoundsSize.Y / 2); IndexY < BoundsSize.Y / 2 + (int)BoundsSize.Y % 2; IndexY++)
			{
				FIntPoint GridLocation = FIntPoint(IndexX, IndexY) + UGridLibrary::WorldLocationToGridLocation(GetActorLocation());
				LocationsToInstanceIndices.Add(GridLocation, GroundMeshComponent->AddInstance(UGridLibrary::GridTransformToWorldTransform(FGridTransform(GridLocation)) * FTransform(FVector(0, 0, -0.1)), true));
			}
		}
	}
	else
	{
		TopCornerLocation = PlaneSize.GetRotated(ActorAngle);
		MiddleCornerLocation = (PlaneSize * FVector2D(1, -1)).GetRotated(ActorAngle);
		if (FMath::Abs(MiddleCornerLocation.X) > FMath::Abs(TopCornerLocation.X))
		{
			FVector2D Temp = TopCornerLocation;
			TopCornerLocation = MiddleCornerLocation;
			MiddleCornerLocation = Temp;
		}
		if (TopCornerLocation.X < 0)
		{
			TopCornerLocation *= -1;
		}
		if (MiddleCornerLocation.Y > 0)
		{
			MiddleCornerLocation *= -1;
		}

		RowOffset = (MiddleCornerLocation - TopCornerLocation);
		RowOffset = FVector2D(UGridLibrary::GetGridHeight(), (UGridLibrary::GetGridHeight() / RowOffset.X) * RowOffset.Y);


		UE_LOG(LogTemp, Warning, TEXT("Row ooff +: %s  |  Top: %s"), *RowOffset.ToString(), *TopCornerLocation.ToString())

		for (FVector2D RowStartLocation = TopCornerLocation; RowStartLocation.SizeSquared() <= TopCornerLocation.SizeSquared(); RowStartLocation -= RowOffset)
		{
			FIntPoint GridLocation = UGridLibrary::WorldLocationToGridLocation(GetActorLocation() + FVector(RowStartLocation, 0));
			LocationsToInstanceIndices.Add(GridLocation, GroundMeshComponent->AddInstance(UGridLibrary::GridTransformToWorldTransform(FGridTransform(GridLocation)) * FTransform(FVector(0, 0, -0.1)), true));
		}
		RowOffset.Y = -(UGridLibrary::GetGridHeight() / RowOffset.Y) * RowOffset.X;
		for (FVector2D RowStartLocation = MiddleCornerLocation - RowOffset; RowStartLocation.SizeSquared() <= MiddleCornerLocation.SizeSquared(); RowStartLocation -= RowOffset)
		{
			FIntPoint GridLocation = UGridLibrary::WorldLocationToGridLocation(GetActorLocation() + FVector(RowStartLocation, 0));
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