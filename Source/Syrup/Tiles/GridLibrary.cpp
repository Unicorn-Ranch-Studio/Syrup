// Fill out your copyright notice in the Description page of Project Settings.


#include "GridLibrary.h"

#include "Tile.h"

/*
 * Gets the world transform of a grid location.
 *
 * @param Location - The location on the grid to get the tranform of.
 * @return The world transform of a grid location.
 */
FTransform UGridLibrary::GridLocationToWorldTransform(FIntPoint Location)
{
	double XLocation = GetGridHeight() * Location.X + (!IsGridLocationFlipped(Location) ? GetGridHeight() *.333333333333 : GetGridHeight() * .666666666666);
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
 * Gets snaps a given location to the grid.
 *
 * @param Location - The location in the world to snap.
 * @return The snaped transform.
 */
FTransform UGridLibrary::SnapWorldLocationToGrid(FVector Location)
{
	return GridLocationToWorldTransform(WorldLocationToGridLocation(Location));
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
EGridDirection UGridLibrary::GetNextDirection(EGridDirection Direction, bool bCounterClockwise)
{
	return (EGridDirection)(((uint8)Direction + (bCounterClockwise ? 4 : 2)) % 6);
}

/*
 * Gets the opposite grid direction of a given grid direction.
 *
 * @param Direction - The given direction.
 * @return The opposite grid direction of a given grid direction.
 */
EGridDirection UGridLibrary::FlipDirection(EGridDirection Direction)
{
	uint8 IntDirection = (uint8)Direction;
	return (EGridDirection)((IntDirection + (IntDirection % 2 ? -1 : 1)));
}

/*
 * Gets whether or not a given direction is valid at a given location.
 *
 * @param Direction - The given direction.
 * @param Location - The given location.
 * @return Whether or not a given direction is valid at a given location.
 */
bool UGridLibrary::IsDirectionValidAtLocation(EGridDirection Direction, FIntPoint Location)
{
	return (uint8)Direction % 2 == IsGridLocationFlipped(Location);
}

/*
 * Gets where the a given relative location of a tile would be if its root was pointed in a given direction. Intial direction assumed to be up.
 *
 * @param Direction - The given direction.
 * @param Location - The given location.
 * @return Where the a given relative location of a tile would be if its root was pointed in a given direction.
 */
FIntPoint UGridLibrary::PointLocationInDirection(EGridDirection Direction, FIntPoint Location)
{
	// Gets the world location of the location to rotate.
	FVector WorldLocation = GridLocationToWorldTransform(Location).GetTranslation() - FVector(GetGridHeight() * 0.33333333333333, 0, 0);
	// Rotates that location in world space
	WorldLocation = WorldLocation.RotateAngleAxis(((uint8)Direction / 2) * 120, FVector(0, 0, 1));
	// Translates that back to grid space
	Location =  WorldLocationToGridLocation(WorldLocation + FVector(GetGridHeight() * 0.33333333333333, 0, 0));

	// Flip if Nessesary
	if (IsDirectionValidAtLocation(Direction, FIntPoint::ZeroValue))
	{
		Location.X = -Location.X;
	}

	return Location;
}

/*
 * Gets where the a given set of relative locations of a shape would be if its root was pointed in a given direction. Intial direction assumed to be up.
 *
 * @param Direction - The given direction.
 * @param Location - The given set of  locations.
 * @return Where the a given relative location of a shape would be if its root was pointed in a given direction.
 */
TSet<FIntPoint> UGridLibrary::PointShapeInDirection(EGridDirection Direction, TSet<FIntPoint> TileLocations)
{
	for (FIntPoint& EachTileLocation : TileLocations)
	{
		EachTileLocation = PointLocationInDirection(Direction, EachTileLocation);
	}
	return TileLocations;
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

/*
 * Gets all the grid locations of a given shape when scaled up.
 *
 * @param ShapeLocations - The locations contained shape to scale.
 * @param Size -  The number of layers to add to the shape.
 * @return All the grid locations of a given shape when scaled up.
 */
TSet<FIntPoint> UGridLibrary::ScaleShapeUp(TSet<FIntPoint> ShapeLocations, int Size, bool bChopPoints)
{
	// End if invalid size
	if (Size < 1)
	{
		return ShapeLocations;
	}

	// Get the details of the layers that need to be added
	TSet<TTuple<FIntPoint, EGridDirection, bool>> LayerDetails = TSet<TTuple<FIntPoint, EGridDirection, bool>>();
	for (FIntPoint EachShapeLocation : ShapeLocations)
	{
		// Get the adjacent locations of the shape and check to see if they are not contained in the shape
		TMap<EGridDirection, FIntPoint> AdjacentLocations = GetAdjacentGridLocations(EachShapeLocation);
		for (EGridDirection DirectionIndex = (EGridDirection)IsGridLocationFlipped(EachShapeLocation); (uint8)DirectionIndex < 6; DirectionIndex = (EGridDirection)((uint8)DirectionIndex + 2))
		{
			bool bShouldLayerBeAdded = !ShapeLocations.Contains(AdjacentLocations.FindRef(DirectionIndex));
			if (bShouldLayerBeAdded)
			{
				// If next adjacent location is also outside the shape then add cap
				bool bShouldCapBeAdded = bShouldLayerBeAdded && !ShapeLocations.Contains(AdjacentLocations.FindRef(GetNextDirection(DirectionIndex)));

				LayerDetails.Add(TTuple<FIntPoint, EGridDirection, bool>(AdjacentLocations.FindRef(DirectionIndex), DirectionIndex, bShouldCapBeAdded));
			}
		}
	}


	// Create Layers
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>(ShapeLocations);
	
	// For each layer details get the locations of a trapziod with one corner on the layer location.
	for (TTuple<FIntPoint, EGridDirection, bool> EachLayerDetail : LayerDetails)
	{
		TArray<FIntPoint> StartLocations = (GetLocationsInLine(EachLayerDetail.Get<FIntPoint>(), GetNextDirection(EachLayerDetail.Get<EGridDirection>(), true), 2 * Size)).Array();
		
		for (int LayerIndex = 0; LayerIndex < Size; LayerIndex++)
		{
			// Adjust trapizode size if a cap is needed.
			int Length = 2 + 2 * LayerIndex;
			if (EachLayerDetail.Get<bool>())
			{
				Length += 2 * (Size);
				if (bChopPoints)
				{
					Length -= LayerIndex * 2 + 1;
				}
			}

			ReturnValue = ReturnValue.Union(GetLocationsInLine(StartLocations[1 + 2 * LayerIndex], EachLayerDetail.Get<EGridDirection>(), -Length));
		}
	}

	return ReturnValue;
}

/*
 * Gets all the grid locations of a line.
 *
 * @param LineOrigin - The start of the line.
 * @param Size -  The direction perpendicular to the line.
 * @param Length - The length of he line.
 * @param LineStartOffset - The location along the line to start drawing it at.
 * @return All the grid locations of a line.
 */
TSet<FIntPoint> UGridLibrary::GetLocationsInLine(FIntPoint LineOrigin, EGridDirection PerpendicularDirection, int Length, int LineStartOffset)
{
	// Snap direction to location.
	if (IsDirectionValidAtLocation(PerpendicularDirection, LineOrigin))
	{
		PerpendicularDirection = FlipDirection(PerpendicularDirection);
	}

	// Get the direction the line will move in based on direction.
	FIntPoint LineDirection = FIntPoint::ZeroValue;
	FIntPoint LineDirectionFliped = FIntPoint::ZeroValue;
	switch (PerpendicularDirection)
	{
	case EGridDirection::Down:
		LineDirection = FIntPoint(0,-1);
		LineDirectionFliped = LineDirection;
		break;

	case EGridDirection::Up:
		LineDirection = FIntPoint(0, 1);
		LineDirectionFliped = LineDirection;
		break;

	case EGridDirection::UpLeft:
		LineDirection = FIntPoint(0, 1);
		LineDirectionFliped = FIntPoint(1, 0);
		break;

	case EGridDirection::DownRight:
		LineDirection = FIntPoint(-1, 0);
		LineDirectionFliped = FIntPoint(0, -1);
		break;

	case EGridDirection::UpRight:
		LineDirection = FIntPoint(-1, 0);
		LineDirectionFliped = FIntPoint(0, 1);
		break;

	case EGridDirection::DownLeft:
		LineDirection = FIntPoint(0, -1);
		LineDirectionFliped = FIntPoint(1, 0);
		break;
	}

	// Adjust line origin based off of offset.
	while (LineStartOffset != 0)
	{
		LineOrigin += FIntPoint(FMath::Sign(LineStartOffset)) * ((IsGridLocationFlipped(LineOrigin) != LineStartOffset < 0) ? LineDirectionFliped : LineDirection);
		LineStartOffset -= FMath::Sign(LineStartOffset);
	}

	//Get line locations.
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>();
	ReturnValue.Add(LineOrigin);
	while (Length != 0)
	{
		LineOrigin += FIntPoint(FMath::Sign(Length)) * ((IsGridLocationFlipped(LineOrigin) != Length < 0) ? LineDirectionFliped : LineDirection);
		ReturnValue.Add(LineOrigin);
		Length -= FMath::Sign(Length);
	}

	return ReturnValue;
}

/**
 * Checks a given grid location for a tile.
 *
 * @param WorldContext - The an object in world to check.
 * @param GridLocation - The given grid location to check.
 * @param OverlapingTile - Will be set to the tile at the given location if there is one, otherwise is nullptr.
 * @param IgnoredTiles - The tiles to ignore when querying.
 * @return Whether or not a tile was at the given location.
 */
bool UGridLibrary::OverlapGridLocation(const UObject* WorldContext, const FIntPoint GridLocation, ATile*& OverlapingTile, const TArray<AActor*>& IgnoredTiles)
{
	FCollisionQueryParams Params = FCollisionQueryParams();
	Params.AddIgnoredActors(IgnoredTiles);

	FHitResult Hit = FHitResult();

	FVector WorldLocation = GridLocationToWorldTransform(GridLocation).GetTranslation();

	bool ReturnValue = WorldContext->GetWorld()->LineTraceSingleByObjectType(Hit, WorldLocation, WorldLocation - FVector(0, 0, KINDA_SMALL_NUMBER), FCollisionObjectQueryParams::AllDynamicObjects, Params);
	OverlapingTile = Cast<ATile>(Hit.GetActor());
	return IsValid(OverlapingTile);
}

/**
 * Checks a given shape's grid locations for tiles.
 *
 * @param WorldContext - The an object in world to check.
 * @param ShapeGridLocations - The given shape's grid locations to check.
 * @param OverlapingTiles - Will be set to the tile at the given location if there is one, otherwise is nullptr.
 * @param IgnoredTiles - The tiles to ignore when querying.
 * @return Whether or not a tile was at overlaping the given shape.
 */
bool UGridLibrary::OverlapShape(const UObject* WorldContext, const TSet<FIntPoint>& ShapeGridLocations, TSet<ATile*>& OverlapingTiles, const TArray<AActor*>& IgnoredTiles)
{
	for (FIntPoint EachShapeGridLocation : ShapeGridLocations)
	{
		ATile* OverlapedTile;
		OverlapGridLocation(WorldContext, EachShapeGridLocation, OverlapedTile, IgnoredTiles);
		if (IsValid(OverlapedTile))
		{
			OverlapingTiles.Add(OverlapedTile);
		}
	}
	return (bool)(OverlapingTiles.Num());
}