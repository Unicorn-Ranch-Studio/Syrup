// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridLibrary.generated.h"

/**
 * A library for functions related to the in game grid.
 */
UCLASS()
class SYRUP_API UGridLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	/*
	 * Gets the world transform of a grid location.
	 * 
	 * @param Location - The location on the grid to get the tranform of.
	 * @return The world transform of a grid location.
	 */
	UFUNCTION(BlueprintPure, Category="Transformation|Grid")
	FTransform GridLocationToWorldTransform(FIntPoint Location);

	/*
	 * Gets the grid location of a world location.
	 * 
	 * @param Location - The location in the world to get the grid location of.
	 * @return The grid location of a world location.
	 */
	UFUNCTION(BlueprintPure, Category="Transformation|Grid")
	FIntPoint WorldLocationToGridLocation(FVector Location);

	/*
	 * Gets the height of a single grid tile.
	 * 
	 * @return The height of a single grid tile.
	 */
	UFUNCTION(BlueprintPure, Category="Transformation|Grid")
	float GetGridHeight();

	/*
	 * Gets the side length of a single grid tile.
	 * 
	 * @return The side length of a single grid tile.
	 */
	UFUNCTION(BlueprintPure, Category="Transformation|Grid")
	float GetGridSideLength();

	/*
	 * Gets whether or not a tile at a given grid location will be fliped.
	 *
	 * @param Location - The location of the tile.
	 * @return Whether or not a tile at a given grid location will be fliped.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid")
	bool IsGridLocationFlipped(FIntPoint Location);

	/*
	 * Gets all the grid locations adjacent to a given grid location.
	 *
	 * @param Location - The given location.
	 * @param Iterations - The number of adjacency iterations to compute.
	 * @return All the locations adjacent to a given location.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Adjacency")
	TSet<FIntPoint> GetAdjacentGridLocations(FIntPoint Location, int Iterations = 1);

	/*
	 * Gets all the grid locations within a radius of a given grid location.
	 *
	 * @param Location - The given location.
	 * @param Radius -  The Radius to get locations within
	 * @return All the grid locations within a radius of a given grid location.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Adjacency")
	TSet<FIntPoint> GetGridLocationsInRadius(FIntPoint Location, float Radius);


private:
	UPROPERTY()
	float GridHeight = 3;
};
