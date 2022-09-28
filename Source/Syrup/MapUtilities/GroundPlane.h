// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FieldType.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroundPlane.generated.h"

class ATile;

/* \/ ============== \/ *\
|  \/ AGroundPlane \/  |
\* \/ ============== \/ */
/**
 * An object for portraying the ground plain.
 * Has functions for receiving field data.
 */
UCLASS()
class SYRUP_API AGroundPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	//The size of the grid to track fields in.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint PlaneSize = FIntPoint(100, 100);

	//The grid offset of this plane.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint Offset = FIntPoint(0, 0);

	/**
	 * Applies a field in a given area.
	 *
	 * @param FieldType - The type of the field. Fields of different types are independent.
	 * @param Locations - The locations to add the field in.
	 * 
	 * @return If this plane was in any of the effected locations.
	 */
	UFUNCTION(BlueprintCallable)
	bool ApplyField(const EFieldType Type, const TSet<FIntPoint>& Locations);

	/**
	 * Removes a field from a given area.
	 *
	 * @param FieldType - The type of the field. Fields of different types are independent.
	 * @param Locations - The locations to remove the field from.
	 * 
	 * @return If this plane was in any of the effected locations.
	 */
	UFUNCTION(BlueprintCallable)
	bool RemoveField(const EFieldType Type, const TSet<FIntPoint>& Locations);

	/**
	 * Creates and sets up the ground mesh.
	 */
	AGroundPlane();

	/**
	 * Sets the size of the ground mesh.
	 * 
	 * @param Transform - The new transform.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	//The plane used to render the fields
	UPROPERTY()
	UInstancedStaticMeshComponent* GroundMesh;

	//The material to be applied to the ground
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

private:
	/**
	 * Edits a field's strength in the given area.
	 * 
	 * @param FieldType - The type of the field. Fields of different types have independent strengths.
	 * @param Strength - The value to add to the field strength. Note: Field values clamped to >= 0.
	 * @param Locations - The locations to change the field strength in.
	 * 
	 * @return If this plane was in any of the effected locations.
	 */
	UFUNCTION(BlueprintCallable)
	bool AddFieldStrength(const EFieldType FieldType, const int Strength, const TSet<FIntPoint>& Locations);

	//Stores the grid locations of each ground mesh instance
	UPROPERTY()
	TMap<FIntPoint, int32> LocationsToInstanceIndices = TMap<FIntPoint, int32>();

	//Stores strengths of all fields.
	TMap<EFieldType, TMap<FIntPoint, int>> FieldTypeToLocationToStrengths = TMap<EFieldType, TMap<FIntPoint, int>>();
};
/* /\ ============== /\ *\
|  /\ AGroundPlane /\  |
\* /\ ============== /\ */