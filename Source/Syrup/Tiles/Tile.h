// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridLibrary.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class SYRUP_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATile();

	virtual void OnConstruction(const FTransform& Transform) override;

	/**
	 * Gets the grid transform this tile.
	 *
	 * @return The grid transform this tile.
	 */
	UFUNCTION(BlueprintPure)
	FGridTransform GetGridTransform() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* TileMaterial;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UInstancedStaticMeshComponent* SubtileMesh;

private:
	//The relative locations of all of the tiles on this tile.
	UPROPERTY(EditDefaultsOnly)
	TSet<FIntPoint> TileLocations = TSet<FIntPoint>();

	//The mesh used for each tile as the ground.
	UPROPERTY()
	UStaticMesh* TileMesh;
};
