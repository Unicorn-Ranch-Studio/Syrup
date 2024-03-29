// Fill out your copyright notice in the Description page of Project Settings.


#include "GridLibrary.h"

#include "Tile.h"


/* \/ ============ \/ *\
|  \/ UGridLibrary \/  |
\* \/ ============ \/ */
/*
 * Gets the world transform of a grid transform.
 *
 * @param GridTransform - The transform on the grid to get the transform of.
 * @return The world transform of the grid transform.
 */
FTransform UGridLibrary::GridTransformToWorldTransform(const FGridTransform GridTransform)
{
	FIntPoint Location = GridTransform.Location;
	bool bIsFlipped = IsGridLocationFlipped(Location);
	int Offset = (bIsFlipped ? 180 : 120);
	int Direction = (bIsFlipped ? 120 : -120);
	int Multiplier = (int)(IsDirectionValidAtLocation(GridTransform.Direction, Location) ? FlipDirection(GridTransform.Direction) : GridTransform.Direction);
	FRotator Rotation = FRotator(0, Offset + Direction * Multiplier, 0);

	return FTransform(Rotation, GridLocationToWorldLocation(Location));
}

/*
 * Gets the world location of a grid location.
 *
 * @param Location - The transform on the grid to get the transform of.
 * @return The world transform of the grid transform.
 */
FVector UGridLibrary::GridLocationToWorldLocation(const FIntPoint GridLocation)
{
	double XLocation = GetGridHeight() * GridLocation.X + (!IsGridLocationFlipped(GridLocation) ? GetGridHeight() * .333333333333 : GetGridHeight() * .666666666666);
	double YLocation = GetGridSideLength() * GridLocation.Y * 0.5;

	return FVector(XLocation, YLocation, 0);
}

/*
 * Gets the grid transform of a world transform.
 *
 * @param WorldTransform - The transform in the world to get the grid location of.
 * @return The grid transform of the world transform.
 */
FGridTransform UGridLibrary::WorldTransformToGridTransform(const FTransform WorldTransform)
{
	//Scale to grid
	FVector2D GridLocation = FVector2D(WorldTransform.GetLocation()) / FVector2D(GetGridHeight(), GetGridSideLength() * 0.5);
	GridLocation.Y += 0.5;

	//Get the distance from the appropriate edge.
	FVector2D RelativeLocation = FVector2D(FMath::Fractional(GridLocation.X), FMath::Fractional(GridLocation.Y));
	if (RelativeLocation.X < 0)
	{
		RelativeLocation.X = 1 + RelativeLocation.X;
	}
	if (RelativeLocation.Y < 0)
	{
		RelativeLocation.Y = 1 + RelativeLocation.Y;
	}

	//Floor to grid location
	FIntPoint ApproximateLocation = FIntPoint(FMath::Floor(GridLocation.X), FMath::Floor(GridLocation.Y));
	bool bIsFlipped = IsGridLocationFlipped(ApproximateLocation);

	//Adjust for points
	if (RelativeLocation.X < 0.5 == bIsFlipped)
	{
		if (bIsFlipped)
		{
			RelativeLocation.X = 1 - RelativeLocation.X;
		}

		if (RelativeLocation.Y < RelativeLocation.X - 0.5 || 1 - RelativeLocation.Y < RelativeLocation.X - 0.5)
		{
			ApproximateLocation = ApproximateLocation + (RelativeLocation.Y > 0.5 ? FIntPoint(0, 1) : FIntPoint(0, -1));
		}
	}

	//Get grid direction
	bIsFlipped = IsGridLocationFlipped(ApproximateLocation);
	float Yaw = WorldTransform.GetRotation().Rotator().Yaw * (bIsFlipped ? -1 : 1) + (bIsFlipped ? 240 : 60);
	Yaw = Yaw < 0 ? 360 + Yaw : Yaw;
	Yaw = FMath::Fmod(Yaw, 360);
	EGridDirection Direction = (EGridDirection)(((int)(Yaw / 120) * 2) + (bIsFlipped ? 0 : 1));

	return FGridTransform(ApproximateLocation, Direction);
}

/*
 * Gets the grid location of a world location.
 *
 * @param WorldLocation - The transform in the world to get the grid location of.
 * @return The grid location of the world location.
 */
FIntPoint UGridLibrary::WorldLocationToGridLocation(const FVector WorldLocation)
{
	//Scale to grid
	FVector2D GridLocation = FVector2D(WorldLocation) / FVector2D(GetGridHeight(), GetGridSideLength() * 0.5);
	GridLocation.Y += 0.5;

	//Get the distance from the appropriate edge.
	FVector2D RelativeLocation = FVector2D(FMath::Fractional(GridLocation.X), FMath::Fractional(GridLocation.Y));
	if (RelativeLocation.X < 0)
	{
		RelativeLocation.X = 1 + RelativeLocation.X;
	}
	if (RelativeLocation.Y < 0)
	{
		RelativeLocation.Y = 1 + RelativeLocation.Y;
	}

	//Floor to grid location
	FIntPoint ApproximateLocation = FIntPoint(FMath::Floor(GridLocation.X), FMath::Floor(GridLocation.Y));
	bool bIsFlipped = IsGridLocationFlipped(ApproximateLocation);

	//Adjust for points
	if (RelativeLocation.X < 0.5 == bIsFlipped)
	{
		if (bIsFlipped)
		{
			RelativeLocation.X = 1 - RelativeLocation.X;
		}

		if (RelativeLocation.Y < RelativeLocation.X - 0.5 || 1 - RelativeLocation.Y < RelativeLocation.X - 0.5)
		{
			ApproximateLocation = ApproximateLocation + (RelativeLocation.Y > 0.5 ? FIntPoint(0, 1) : FIntPoint(0, -1));
		}
	}

	return ApproximateLocation;
}

/*
 * Transforms the given grid location by the grid transform.
 *
 * @param GridLocation - The location to transform.
 * @param GridTransform - The transformation to apply.
 * @return The transformed location.
 */
FIntPoint UGridLibrary::TransformGridLocation(const FIntPoint GridLocation, const FGridTransform GridTransform)
{
	return PointLocationInDirection(GridTransform.Direction, GridLocation) + GridTransform.Location;
}

/*
 * Transforms the given shape by the grid transform.
 *
 * @param ShapeLocations - The location of each tile in the shape to transform.
 * @param GridTransform - The transformation to apply.
 * @return The transformed location of each tile in the shape.
 */
TSet<FIntPoint> UGridLibrary::TransformShape(const TSet<FIntPoint> ShapeLocations, const FGridTransform GridTransform)
{
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>();
	for (FIntPoint EachShapeLocation : ShapeLocations)
	{
		ReturnValue.Add(TransformGridLocation(EachShapeLocation, GridTransform));
	}
	return ReturnValue;
}

/*
 * Gets snaps a given transform to the grid.
 *
 * @param Location - The transform in the world to snap.
 * @return The snaped transform.
 */
FTransform UGridLibrary::SnapWorldTransformToGrid(const FTransform Transform)
{
	return GridTransformToWorldTransform(WorldTransformToGridTransform(Transform));
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
 * Gets whether or not a tile at a given grid location will be flipped.
 *
 * @param Location - The location of the tile.
 * @return Whether or not a tile at a given grid location will be flipped.
 */
bool UGridLibrary::IsGridLocationFlipped(const FIntPoint Location)
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
EGridDirection UGridLibrary::GetNextDirection(const EGridDirection Direction, const bool bCounterClockwise)
{
	return (EGridDirection)(((uint8)Direction + (bCounterClockwise ? 4 : 2)) % 6);
}

/*
 * Gets the opposite grid direction of a given grid direction.
 *
 * @param Direction - The given direction.
 * @return The opposite grid direction of a given grid direction.
 */
EGridDirection UGridLibrary::FlipDirection(const EGridDirection Direction)
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
bool UGridLibrary::IsDirectionValidAtLocation(const EGridDirection Direction, const FIntPoint Location)
{
	return (uint8)Direction % 2 == IsGridLocationFlipped(Location);
}

/*
 * Gets where the a given relative location of a tile would be if its root was pointed in a given direction. Initial direction assumed to be up.
 *
 * @param Direction - The given direction.
 * @param Location - The given location.
 * @return Where the a given relative location of a tile would be if its root was pointed in a given direction.
 */
FIntPoint UGridLibrary::PointLocationInDirection(const EGridDirection Direction, const FIntPoint Location)
{
	FIntPoint ReturnValue = Location;

	// Flip if Necessary
	if (IsDirectionValidAtLocation(Direction, FIntPoint::ZeroValue))
	{
		ReturnValue.X = -ReturnValue.X + 1;
		ReturnValue.Y = -ReturnValue.Y;

		FVector Offset = FVector(GetGridHeight() * 1.666666666666666, 0, 0);

		// Gets the world location of the location to rotate.
		FVector WorldLocation = GridTransformToWorldTransform(ReturnValue).GetTranslation() - Offset;
		// Rotates that location in world space.
		WorldLocation =	 WorldLocation.RotateAngleAxis(((uint8)Direction / 2) * -120, FVector(0, 0, 1));
		// Translates that back to grid space.
		ReturnValue = WorldTransformToGridTransform(FTransform(WorldLocation + Offset)).Location;
		ReturnValue.X = ReturnValue.X - 1;
	}
	else
	{
		FVector Offset = FVector(GetGridHeight() * 0.333333333333333, 0, 0);

		// Gets the world location of the location to rotate.
		FVector WorldLocation = GridTransformToWorldTransform(ReturnValue).GetTranslation() - Offset;
		// Rotates that location in world space
		WorldLocation =  WorldLocation.RotateAngleAxis(((uint8)Direction / 2) * 120, FVector(0, 0, 1));
		// Translates that back to grid space
		ReturnValue = WorldTransformToGridTransform(FTransform(WorldLocation + Offset)).Location;

	}

	return ReturnValue;
}

/*
 * Gets where the a given set of relative locations of a shape would be if its root was pointed in a given direction. Initial direction assumed to be up.
 *
 * @param Direction - The given direction.
 * @param Location - The given set of  locations.
 * @return Where the a given relative location of a shape would be if its root was pointed in a given direction.
 */
TSet<FIntPoint> UGridLibrary::PointShapeInDirection(const EGridDirection Direction, const TSet<FIntPoint> TileLocations)
{
	TSet<FIntPoint> RotatedTileLocations = TileLocations;
	for (FIntPoint& EachTileLocation : RotatedTileLocations)
	{
		EachTileLocation = PointLocationInDirection(Direction, EachTileLocation);
	}
	return RotatedTileLocations;
}

/*
 * Gets all the grid locations adjacent to a given grid location.
 *
 * @param Location - The given location.
 * @return All the locations adjacent to a given location.
 */
TMap<EGridDirection, FIntPoint> UGridLibrary::GetAdjacentGridLocations(const FIntPoint Location)
{
	TMap<EGridDirection, FIntPoint> AdjecentTiles = TMap<EGridDirection, FIntPoint>();
	bool bFlip = IsGridLocationFlipped(Location);

	AdjecentTiles.Add(bFlip ? EGridDirection::Up        : EGridDirection::Down   , Location + FIntPoint(bFlip ? 1 : -1, 0));
	AdjecentTiles.Add(bFlip ? EGridDirection::DownLeft  : EGridDirection::UpLeft , Location + FIntPoint(0, -1));
	AdjecentTiles.Add(bFlip ? EGridDirection::DownRight : EGridDirection::UpRight, Location + FIntPoint(0, 1));

	return AdjecentTiles;
}

/*
 * Gets all the grid locations within a range of a given grid location.
 *
 * @param Location - The given location.
 * @param Range -  The Range to get locations within.
 * @return All the grid locations within a range of a given grid location.
 */
TSet<FIntPoint> UGridLibrary::GetGridLocationsInRange(const FIntPoint Location, const double Range)
{
	float AbsRange = FMath::Abs(Range);
	FIntPoint SearchArea = FIntPoint(FMath::CeilToDouble(AbsRange), FMath::CeilToDouble(AbsRange * 2 * 0.86602540378));
	double YSize = 1 / (2 * 0.86602540378);
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>();

	for (int IndexX = -SearchArea.X; IndexX <= 0; IndexX++)
	{
		for (int IndexY = -SearchArea.Y; IndexY <= 0; IndexY++)
		{
			if (IndexX * IndexX + IndexY * IndexY * YSize * YSize < AbsRange * AbsRange)
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
TSet<FIntPoint> UGridLibrary::ScaleShapeUp(const TSet<FIntPoint>& ShapeLocations, const int Size, const bool bChopPoints)
{
	TSet<FIntPoint> ShapeLocationsRehashed = TSet<FIntPoint>(ShapeLocations.Array());

	// End if invalid size
	if (Size < 1)
	{
		return ShapeLocationsRehashed;
	}

	// Get the details of the layers that need to be added
	TSet<TTuple<FIntPoint, EGridDirection, bool>> LayerDetails = TSet<TTuple<FIntPoint, EGridDirection, bool>>();
	for (FIntPoint EachShapeLocation : ShapeLocationsRehashed)
	{
		// Get the adjacent locations of the shape and check to see if they are not contained in the shape
		TMap<EGridDirection, FIntPoint> AdjacentLocations = GetAdjacentGridLocations(EachShapeLocation);
		for (EGridDirection DirectionIndex = (EGridDirection)IsGridLocationFlipped(EachShapeLocation); (uint8)DirectionIndex < 6; DirectionIndex = (EGridDirection)((uint8)DirectionIndex + 2))
		{
			bool bShouldLayerBeAdded = !ShapeLocationsRehashed.Contains(AdjacentLocations.FindRef(DirectionIndex));
			if (bShouldLayerBeAdded)
			{
				// If next adjacent location is also outside the shape then add cap
				bool bShouldCapBeAdded = !ShapeLocationsRehashed.Contains(AdjacentLocations.FindRef(GetNextDirection(DirectionIndex)));
				
				LayerDetails.Add(TTuple<FIntPoint, EGridDirection, bool>(AdjacentLocations.FindRef(DirectionIndex), DirectionIndex, bShouldCapBeAdded));
			}
		}
	}


	// Create Layers
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>(ShapeLocationsRehashed);
	
	// For each layer details get the locations of a trapezoid with one corner on the layer location.
	for (TTuple<FIntPoint, EGridDirection, bool> EachLayerDetail : LayerDetails)
	{
		TArray<FIntPoint> StartLocations = (GetLocationsInLine(EachLayerDetail.Get<FIntPoint>(), GetNextDirection(EachLayerDetail.Get<EGridDirection>(), true), 2 * Size)).Array();
		
		for (int LayerIndex = 0; LayerIndex < Size; LayerIndex++)
		{
			// Adjust trapezoid size if a cap is needed.
			int Length = 2 + 2 * LayerIndex;
			if (EachLayerDetail.Get<bool>())
			{
				Length += 2 * (Size);
				if (bChopPoints)
				{
					Length -= LayerIndex * 2 + 1;
				}
			}

			ReturnValue = ReturnValue.Union(GetLocationsInLine(StartLocations[1 + 2 * LayerIndex], EachLayerDetail.Get<EGridDirection>(), Length));
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
TSet<FIntPoint> UGridLibrary::GetLocationsInLine(const FIntPoint LineOrigin, const EGridDirection PerpendicularDirection, const int Length, const int LineStartOffset)
{
	// Snap direction to location.
	EGridDirection Direction = IsDirectionValidAtLocation(PerpendicularDirection, LineOrigin) ? FlipDirection(PerpendicularDirection) : PerpendicularDirection;

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
	FIntPoint LineLocation = LineOrigin;
	int RemainingOffset = LineStartOffset;
	while (RemainingOffset != 0)
	{
		LineLocation += FIntPoint(FMath::Sign(RemainingOffset)) * ((IsGridLocationFlipped(LineLocation) != RemainingOffset < 0) ? LineDirectionFliped : LineDirection);
		RemainingOffset -= FMath::Sign(RemainingOffset);
	}

	//Get line locations.
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>();
	ReturnValue.Add(LineLocation);
	int RemainingLength = Length;
	while (RemainingLength != 0)
	{
		LineLocation += FIntPoint(FMath::Sign(RemainingLength)) * ((IsGridLocationFlipped(LineLocation) != RemainingLength < 0) ? LineDirectionFliped : LineDirection);
		ReturnValue.Add(LineLocation);
		RemainingLength -= FMath::Sign(RemainingLength);
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
 * @param Channel - The channel to test overlaps against.
 * 
 * @return Whether or not a tile was at the given location.
 */
bool UGridLibrary::OverlapGridLocation(const UObject* WorldContext, const FIntPoint GridLocation, ATile*& OverlapingTile, const TArray<AActor*>& IgnoredTiles, const ECollisionChannel Channel)
{
	FCollisionQueryParams Params = FCollisionQueryParams();
	Params.AddIgnoredActors(IgnoredTiles);
	FHitResult Hit = FHitResult();
	OverlapingTile = nullptr;

	FVector WorldLocation = GridTransformToWorldTransform(GridLocation).GetTranslation();
	if (!WorldContext->GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation + FVector(0, 0, 100), WorldLocation - FVector(0, 0, 0.05), Channel, Params))
	{
		return false;
	}

	OverlapingTile = Cast<ATile>(Hit.GetActor());
	return true;
}

/**
 * Checks a given shape's grid locations for tiles.
 *
 * @param WorldContext - The an object in world to check.
 * @param ShapeGridLocations - The given shape's grid locations to check.
 * @param OverlapingTiles - Will be set to the tile at the given location if there is one, otherwise is nullptr.
 * @param IgnoredTiles - The tiles to ignore when querying.
 * @param Channel - The channel to test overlaps against.
 * 
 * @return Whether or not a tile was at overlapping the given shape.
 */
bool UGridLibrary::OverlapShape(const UObject* WorldContext, const TSet<FIntPoint>& ShapeGridLocations, TSet<ATile*>& OverlapingTiles, const TArray<AActor*>& IgnoredTiles, const ECollisionChannel Channel)
{
	bool bTileOverlaped = false;
	OverlapingTiles = TSet<ATile*>();
	for (FIntPoint EachShapeGridLocation : ShapeGridLocations)
	{
		ATile* OverlapedTile = nullptr;
		bTileOverlaped = OverlapGridLocation(WorldContext, EachShapeGridLocation, OverlapedTile, IgnoredTiles, Channel) || bTileOverlaped;
		if (IsValid(OverlapedTile))
		{
			OverlapingTiles.Add(OverlapedTile);
		}
	}
	return bTileOverlaped;
}
/* /\ ============ /\ *\
|  /\ UGridLibrary /\  |
\* /\ ============ /\ */