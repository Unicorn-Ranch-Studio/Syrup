// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyField.h"

#include "Syrup/Tiles/Tile.h"
#include "EngineUtils.h"

/* \/ =========== \/ *\
|  \/ UApplyField \/  |
\* \/ =========== \/ */
/*
 * Causes this effect.
 *
 * @param EffectedLocations - The locations to effect.
 * @param EffectedTiles - The tiles to effect.
 * @param EffectedNonTileLocations - The locations that are not covered by tiles to effect.
 * @param AffecterTile - The tile doing the affecting.
 */
void UApplyField::Affect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile)
{
	if (GroundPlanes.IsEmpty())
	{
		for (TActorIterator<AGroundPlane> Iterator = TActorIterator<AGroundPlane>(AffecterTile->GetWorld()); Iterator; ++Iterator)
		{
			if (IsValid(*Iterator))
			{
				if (Iterator->ApplyField(FieldType, EffectedLocations))
				{
					GroundPlanes.Add(*Iterator);
					break;
				}
			}
		}
	}
	else
	{
		for (AGroundPlane* EachGroundPlane : GroundPlanes)
		{
			if (IsValid(EachGroundPlane))
			{
				EachGroundPlane->ApplyField(FieldType, EffectedLocations);
			}
		}
	}

	for (ATile* EachEffectedTile : EffectedTiles)
	{
		EachEffectedTile->ApplyField(FieldType);
	}
}

/*
 * Undoes the effect of this.
 *
 * @param EffectedLocations - The locations that were effected.
 * @param EffectedTiles - The tiles that were effected.
 * @param EffectedNonTileLocations - The locations that are not covered by tiles to that were effected.
 * @param AffecterTile - The tile doing the affecting.
 */
void UApplyField::Unaffect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile)
{
	if (GroundPlanes.IsEmpty())
	{
		for (TActorIterator<AGroundPlane> Iterator = TActorIterator<AGroundPlane>(AffecterTile->GetWorld()); Iterator; ++Iterator)
		{
			if (IsValid(*Iterator))
			{
				if (Iterator->RemoveField(FieldType, EffectedLocations))
				{
					GroundPlanes.Add(*Iterator);
					break;
				}
			}
		}
	}
	else
	{
		for (AGroundPlane* EachGroundPlane : GroundPlanes)
		{
			if (IsValid(EachGroundPlane))
			{
				EachGroundPlane->RemoveField(FieldType, EffectedLocations);
			}
		}
	}

	for (ATile* EachEffectedTile : EffectedTiles)
	{
		EachEffectedTile->RemoveField(FieldType);
	}
}
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */