// Fill out your copyright notice in the Description page of Project Settings.


#include "GridLibrary.h"

/*
 * Gets the world transform of a grid location.
 *
 * @param Location - The location on the grid to get the tranform of.
 * @return The world transform of a grid location.
 */
FTransform UGridLibrary::GridLocationToWorldTransform(FIntPoint Location)
{
	float XLocation = GetGridHeight() * Location.X - (IsGridLocationFliped(Location) ? GetGridHeight() *.333333333333 : GetGridHeight() * .666666666666);
	float YLocation = GetGridSideLength() * Location.Y;
	FRotator Rotation = FRotator(0, IsGridLocationFliped(Location) ? 180 : 0, 0);

	return FTransform(Rotation, FVector(XLocation, YLocation, 0));
}

/*
 * Gets the grid location of a world location.
 *
 * @param Location - The location in the world to get the grid location of.
 * @return The grid location of a world location.
 */
FIntPoint UGridLibrary::WorldLocationToGridLocation(FVector Location)
{
	FVector2D ScaledLocation = FVector2D(Location) / FVector2D(GetGridHeight(), GetGridSideLength());
	return FIntPoint(FMath::RoundFromZero(ScaledLocation.X), FMath::RoundFromZero(ScaledLocation.Y));
}

/*
 * Gets the height of a single grid tile.
 *
 * @return The height of a single grid tile.
 */
float UGridLibrary::GetGridHeight()
{
	return GridHeight;
}

/*
 * Gets the side length of a single grid tile.
 *
 * @return The side length of a single grid tile.
 */
float UGridLibrary::GetGridSideLength()
{
	return 1.15470053837925152901829756100391491129520350254 * GridHeight;
}

/*
 * Gets whether or not a tile at a given grid location will be fliped.
 *
 * @param Location - The location of the tile.
 * @return Whether or not a tile at a given grid location will be fliped.
 */
bool UGridLibrary::IsGridLocationFliped(FIntPoint Location)
{
	return false;
}

/*
 * Gets all the grid locations adjacent to a given grid location.
 *
 * @param Location - The given location.
 * @param Iterations - The number of adjacency iterations to compute.
 * @return All the locations adjacent to a given location.
 */
TSet<FIntPoint> UGridLibrary::GetAdjacentGridLocations(FIntPoint Location, int Iterations)
{
	return TSet<FIntPoint>();
}

/*
 * Gets all the grid locations within a radius of a given grid location.
 *
 * @param Location - The given location.
 * @param Radius -  The Radius to get locations within
 * @return All the grid locations within a radius of a given grid location.
 */
TSet<FIntPoint> UGridLibrary::GetGridLocationsInRadius(FIntPoint Location, float Radius)
{
	return TSet<FIntPoint>();
}
