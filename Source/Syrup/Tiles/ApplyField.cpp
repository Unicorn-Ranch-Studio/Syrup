// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyField.h"

#include "Tile.h"
#include "EngineUtils.h"

/* \/ =========== \/ *\
|  \/ UApplyField \/  |
\* \/ =========== \/ */
/*
 * Affects the set of all locations this effect.
 *
 * @param EffectedTiles - The locations to effect.
 * @param AffecterTile - The tile doing the affecting.
 */
void UApplyField::AffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile) 
{
	if (!IsValid(GroundPlane))
	{
		for (TActorIterator<AGroundPlane> Iterator = TActorIterator<AGroundPlane>(AffecterTile->GetWorld()); Iterator; ++Iterator)
		{
			if (IsValid(*Iterator))
			{
				GroundPlane = *Iterator;
				break;
			}
		}
	}
	if (IsValid(GroundPlane))
	{
		GroundPlane->ApplyField(FieldType, EffectedLocations);
		UE_LOG(LogTemp, Warning, TEXT("Apply"));
	}
}

/*
 * Affects the set of effected tiles with this effect.
 *
 * @param EffectedTiles - The tiles to effect.
 * @param AffecterTile - The tile doing the affecting.
 */
void UApplyField::AffectTiles(TSet<ATile*> EffectedTiles, ATile* AffecterTile)
{
	for (ATile* EachEffectedTile : EffectedTiles)
	{
		EachEffectedTile->ApplyField(FieldType);
	}
}

/*
 * Undoes the affects of this on the set of a effected locations.
 *
 * @param EffectedLocations - The locations to undo the effect on.
 * @param AffecterTile - The tile doing the affecting.
 */
void UApplyField::UnaffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile) 
{
	if (!IsValid(GroundPlane))
	{
		for (TActorIterator<AGroundPlane> Iterator = TActorIterator<AGroundPlane>(AffecterTile->GetWorld()); Iterator; ++Iterator)
		{
			if (IsValid(*Iterator))
			{
				GroundPlane = *Iterator;
				break;
			}
		}
	}
	if (IsValid(GroundPlane))
	{
		GroundPlane->RemoveField(FieldType, EffectedLocations);
		UE_LOG(LogTemp, Warning, TEXT("Remove"));
	}
}

/*
 * Undoes the affects of this on the set of effected tiles.
 *
 * @param EffectedTiles - The tiles to undo the effect on.
 * @param AffecterTile - The tile doing the affecting.
 */
void UApplyField::UnaffectTiles(TSet<ATile*> EffectedTiles, ATile* AffecterTile)
{
	for (ATile* EachEffectedTile : EffectedTiles)
	{
		EachEffectedTile->RemoveField(FieldType);
	}
}
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */