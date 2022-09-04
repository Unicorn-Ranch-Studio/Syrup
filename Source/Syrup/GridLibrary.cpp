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
	double XLocation = GetGridHeight() * Location.X - (IsGridLocationFlipped(Location) ? GetGridHeight() *.333333333333 : GetGridHeight() * .666666666666);
	double YLocation = GetGridSideLength() * Location.Y * 0.5;
	FRotator Rotation = FRotator(0, IsGridLocationFlipped(Location) ? 180 : 0, 0);

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
	FVector2D GridLocation = FVector2D(Location) / FVector2D(GetGridHeight(), GetGridSideLength() * 0.5);
	GridLocation.Y += 0.5;

	FVector2D RelativeLocation = FVector2D(FMath::Fractional(GridLocation.X), FMath::Fractional(GridLocation.Y));
	if (RelativeLocation.X < 0)
	{
		RelativeLocation.X = 1 + RelativeLocation.X;
	}
	if (RelativeLocation.Y < 0)
	{
		RelativeLocation.Y = 1 + RelativeLocation.Y;
	}

	FIntPoint ApproximateLocation = FIntPoint(FMath::Floor(GridLocation.X), FMath::Floor(GridLocation.Y));

	bool bIsFlipped = IsGridLocationFlipped(ApproximateLocation);
	
	if (RelativeLocation.X < 0.5 != bIsFlipped)
	{
		return ApproximateLocation;
	}

	if (bIsFlipped)
	{
		RelativeLocation.X = 1 - RelativeLocation.X;
	}

	if (RelativeLocation.Y < RelativeLocation.X - 0.5 || 1 - RelativeLocation.Y < RelativeLocation.X - 0.5)
	{
		return ApproximateLocation + (RelativeLocation.Y > 0.5 ? FIntPoint(0, 1) : FIntPoint(0, -1));
	}
	
	return ApproximateLocation;
}

/*
 * Gets the height of a single grid tile.
 *
 * @return The height of a single grid tile.
 */
double UGridLibrary::GetGridHeight()
{
	return Cast<UGridLibrary>(UGridLibrary::StaticClass()->GetDefaultObject())->GridHeight;
}

/*
 * Gets the side length of a single grid tile.
 *
 * @return The side length of a single grid tile.
 */
double UGridLibrary::GetGridSideLength()
{
	return 1.15470053837925152901829756100391491129520350254 * Cast<UGridLibrary>(UGridLibrary::StaticClass()->GetDefaultObject())->GridHeight;
}

/*
 * Gets whether or not a tile at a given grid location will be fliped.
 *
 * @param Location - The location of the tile.
 * @return Whether or not a tile at a given grid location will be fliped.
 */
bool UGridLibrary::IsGridLocationFlipped(FIntPoint Location)
{
	return FMath::Abs(Location.Y % 2) != FMath::Abs(Location.X % 2);
}

/*
 * Gets the grid direction after a given grid direction.
 *
 * @param Direction - The given direction.
 * @param bCounterClockwise - Whether after is defined as being counterclockwise of instead of clockwise of.
 * @return The grid direction after a given grid direction.
 */
EGridDirection UGridLibrary::GetNextDirection(EGridDirection Direction, bool bCounterClockwise = false)
{
	return (EGridDirection)(((uint8)Direction + (bCounterClockwise ? 4 : 2)) % 6);
}

/*
 * Gets all the grid locations adjacent to a given grid location.
 *
 * @param Location - The given location.
 * @return All the locations adjacent to a given location.
 */
TMap<EGridDirection, FIntPoint> UGridLibrary::GetAdjacentGridLocations(FIntPoint Location)
{
	TMap<EGridDirection, FIntPoint> AdjecentTiles = TMap<EGridDirection, FIntPoint>();
	bool bFlip = IsGridLocationFlipped(Location);

	AdjecentTiles.Add(bFlip ? EGridDirection::Up        : EGridDirection::Down   , Location + FIntPoint(bFlip ? 1 : -1, 0));
	AdjecentTiles.Add(bFlip ? EGridDirection::DownLeft  : EGridDirection::UpLeft , Location + FIntPoint(0, -1));
	AdjecentTiles.Add(bFlip ? EGridDirection::DownRight : EGridDirection::UpRight, Location + FIntPoint(0, 1));

	return AdjecentTiles;
}

/*
 * Gets all the grid locations within a radius of a given grid location.
 *
 * @param Location - The given location.
 * @param Radius -  The Radius to get locations within.
 * @return All the grid locations within a radius of a given grid location.
 */
TSet<FIntPoint> UGridLibrary::GetGridLocationsInRadius(FIntPoint Location, double Radius)
{
	Radius = FMath::Abs(Radius);
	FIntPoint SearchArea = FIntPoint(FMath::CeilToDouble(Radius), FMath::CeilToDouble(Radius * 2 * 0.86602540378));
	double YSize = 1 / (2 * 0.86602540378);
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>();

	for (int IndexX = -SearchArea.X; IndexX <= 0; IndexX++)
	{
		for (int IndexY = -SearchArea.Y; IndexY <= 0; IndexY++)
		{
			if (IndexX * IndexX + IndexY * IndexY * YSize * YSize < Radius * Radius)
			{
				ReturnValue.Add(FIntPoint( IndexX,  IndexY));
				ReturnValue.Add(FIntPoint(-IndexX,  IndexY));
				ReturnValue.Add(FIntPoint(-IndexX, -IndexY));
				ReturnValue.Add(FIntPoint( IndexX, -IndexY));
			}
		}
	}

	return ReturnValue;
}
}
