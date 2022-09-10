// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridLibrary.generated.h"

class ATile;

UENUM(BlueprintType)
enum class EGridDirection : uint8
{
	Down		= 0		UMETA(DisplayName = "Down"),
	Up			= 1		UMETA(DisplayName = "Up"),
	UpLeft		= 2		UMETA(DisplayName = "Up and to the Left"),
	DownRight	= 3		UMETA(DisplayName = "Down and to the Right"),
	UpRight		= 4		UMETA(DisplayName = "Up and to the Right"),
	DownLeft	= 5		UMETA(DisplayName = "Down and to the Left")
};

USTRUCT(BlueprintType)
struct SYRUP_API FGridTransform
{
	GENERATED_BODY()

	//The grid location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Location = FIntPoint::ZeroValue;

	//The direction on the grid.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGridDirection Direction = EGridDirection::Up;

	FGridTransform(FIntPoint GridLocation = FIntPoint::ZeroValue, EGridDirection GridDirection = EGridDirection::Up)
	{
		Location = GridLocation;
		Direction = GridDirection;
	}
};

/**
 * A library for functions related to the in game grid.
 */
UCLASS()
class SYRUP_API UGridLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	/*
	 * Gets the world transform of a grid transform.
	 * 
	 * @param Location - The transform on the grid to get the tranform of.
	 * @return The world transform of the grid transform.
	 */
	UFUNCTION(BlueprintPure, Category="Transformation|Grid")
	static FTransform GridTransformToWorldTransform(const FGridTransform GridTransform);

	/*
	 * Gets the grid transform of a world transform.
	 * 
	 * @param WorldTransform - The transform in the world to get the grid location of.
	 * @return The grid transform of the world transform.
	 */
	UFUNCTION(BlueprintPure, Category="Transformation|Grid")
	static FGridTransform WorldTransformToGridTransform(const FTransform WorldTransform);

	/*
	 * Gets snaps a given transform to the grid.
	 * 
	 * @param Location - The transform in the world to snap.
	 * @return The snaped transform.
	 */
	UFUNCTION(BlueprintPure, Category="Transformation|Grid")
	static FTransform SnapWorldTransformToGrid(const FTransform Transform);

	/*
	 * Gets the height of a single grid tile.
	 * 
	 * @return The height of a single grid tile.
	 */
	UFUNCTION(BlueprintPure, Category="Transformation|Grid")
	static double GetGridHeight();

	/*
	 * Gets the side length of a single grid tile.
	 * 
	 * @return The side length of a single grid tile.
	 */
	UFUNCTION(BlueprintPure, Category="Transformation|Grid")
	static double GetGridSideLength();

	/*
	 * Gets whether or not a tile at a given grid location will be fliped.
	 *
	 * @param Location - The location of the tile.
	 * @return Whether or not a tile at a given grid location will be fliped.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid")
	static bool IsGridLocationFlipped(const FIntPoint Location);

	
	/*
	 * Gets the grid direction after a given grid direction.
	 * 
	 * @param Direction - The given direction.
	 * @param bCounterClockwise - Whether after is defined as being counterclockwise of instead of clockwise of.
	 * @return The grid direction after a given grid direction.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Direction")
	static EGridDirection GetNextDirection(const EGridDirection Direction, const bool bCounterClockwise = false);

	
	/*
	 * Gets the opposite grid direction of a given grid direction.
	 * 
	 * @param Direction - The given direction.
	 * @return The opposite grid direction of a given grid direction.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Direction")
	static EGridDirection FlipDirection(const EGridDirection Direction);

	/*
	 * Gets whether or not a given direction is valid at a given location.
	 * 
	 * @param Direction - The given direction.
	 * @param Location - The given location.
	 * @return Whether or not a given direction is valid at a given location.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Direction")
	static bool IsDirectionValidAtLocation(const EGridDirection Direction, const FIntPoint Location);

	/*
	 * Gets where the a given relative location of a tile would be if its root was pointed in a given direction. Intial direction assumed to be up.
	 * 
	 * @param Direction - The given direction.
	 * @param Location - The given location.
	 * @return Where the a given relative location of a tile would be if its root was pointed in a given direction.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Direction")
	static FIntPoint PointLocationInDirection(const EGridDirection Direction, const FIntPoint Location);

	/*
	 * Gets where the a given set of relative locations of a shape would be if its root was pointed in a given direction. Intial direction assumed to be up.
	 *
	 * @param Direction - The given direction.
	 * @param Location - The given set of  locations.
	 * @return Where the a given relative location of a shape would be if its root was pointed in a given direction.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Direction")
	static TSet<FIntPoint> PointShapeInDirection(const EGridDirection Direction, const TSet<FIntPoint> TileLocations);

	/*
	 * Gets all the grid locations adjacent to a given grid location.
	 *
	 * @param Location - The given location.
	 * @param Iterations - The number of adjacency iterations to compute.
	 * @return All the locations adjacent to a given location.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Adjacency")
	static TMap<EGridDirection, FIntPoint> GetAdjacentGridLocations(const FIntPoint Location);

	/*
	 * Gets all the grid locations within a radius of a given grid location.
	 *
	 * @param Location - The given location.
	 * @param Radius -  The Radius to get locations within.
	 * @return All the grid locations within a radius of a given grid location.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Adjacency")
	static TSet<FIntPoint> GetGridLocationsInRadius(const FIntPoint Location, const double Radius);

	/*
	 * Gets all the grid locations of a given shape when scaled up.
	 *
	 * @param ShapeLocations - The locations contained shape to scale.
	 * @param Size -  The number of layers to add to the shape.
	 * @return All the grid locations of a given shape when scaled up.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Adjacency")
	static TSet<FIntPoint> ScaleShapeUp(const TSet<FIntPoint> Shape, const int Size, const bool bChopPoints = false);

	/*
	 * Gets all the grid locations of a line.
	 *
	 * @param LineOrigin - The start of the line.
	 * @param Size -  The direction perpendicular to the line.
	 * @param Length - The length of he line.
	 * @param LineStartOffset - The location along the line to start drawing it at.
	 * @return All the grid locations of a line.
	 */
	UFUNCTION(BlueprintPure, Category = "Transformation|Grid|Adjacency", Meta=(HidePin = "LineStartOffset"))
	static TSet<FIntPoint> GetLocationsInLine(const FIntPoint LineOrigin, const EGridDirection PerpendicularDirection, const int Length = 3, const int LineStartOffset = 0);

	/**
	 * Checks a given grid location for a tile.
	 * 
	 * @param WorldContext - The an object in world to check.
	 * @param GridLocation - The given grid location to check.
	 * @param OverlapingTile - Will be set to the tile at the given location if there is one, otherwise is nullptr.
	 * @param IgnoredTiles - The tiles to ignore when querying.
	 * @return Whether or not a tile was at the given location.
	 */
	UFUNCTION(BlueprintCallable, Category = "Transformation|Grid|Collision", Meta = (WorldContext = "WorldContext", AutoCreateRefTerm = "IgnoredTiles"))
	static bool OverlapGridLocation(const UObject* WorldContext, const FIntPoint GridLocation, ATile*& OverlapingTile, const TArray<AActor*>& IgnoredTiles);
	
	
	/**
	 * Checks a given shape's grid locations for tiles.
	 * 
	 * @param WorldContext - The an object in world to check.
	 * @param ShapeGridLocations - The given shape's grid locations to check.
	 * @param OverlapingTiles - Will be set to the tile at the given location if there is one, otherwise is nullptr.
	 * @param IgnoredTiles - The tiles to ignore when querying.
	 * @return Whether or not a tile was at overlaping the given shape.
	 */
	UFUNCTION(BlueprintCallable, Category = "Transformation|Grid|Collision", Meta=(WorldContext = "WorldContext", AutoCreateRefTerm = "IgnoredTiles"))
	static bool OverlapShape(const UObject* WorldContext, const TSet<FIntPoint>& ShapeGridLocations, TSet<ATile*>& OverlapingTiles, const TArray<AActor*>& IgnoredTiles);

private:
	UPROPERTY()
	double GridHeight = 51.9615242270663188058233902451761710082841576;
};
