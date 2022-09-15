// Fill out your copyright notice in the Description page of Project Settings.


#include "TileAffecterComponent.h"

#include "EngineUtils.h"
#include "TileEffect.h"
#include "Syrup/Tiles/GridLibrary.h"

/* \/ ============= \/ *\
|  \/ AAffecterTile \/  |
\* \/ ============= \/ */
/**
 * Applies all of this affecter's effects. Note effects will only be applied once per effected location/tile.
 */
void UTileAffecterComponent::ApplyEffect()
{
	TSet<ATile*> EffectedTiles;
	TSet<FIntPoint> EffectedNonTileLocations;
	TSet<FIntPoint> EffectedLocations = GetEffectedTilesAndLocations(EffectedTiles, EffectedNonTileLocations);

	ATile* AffectingTile = Cast<ATile>(GetOwner());
	EffectedTiles.Add(AffectingTile);

	EffectedNonTileLocations = EffectedLocations.Difference(LastEffectedLocations);
	EffectedTiles = EffectedTiles.Difference(LastEffectedTiles);
	EffectedNonTileLocations = EffectedNonTileLocations.Difference(LastEffectedNonTileLocations);

	for (TObjectPtr<UTileEffect> EachEffect : Effects)
	{
		if (IsValid(EachEffect))
		{
			EachEffect->Affect(EffectedLocations, EffectedTiles, EffectedNonTileLocations, AffectingTile);
		}
	}

	LastEffectedLocations = EffectedLocations.Union(LastEffectedLocations);
	LastEffectedTiles = EffectedTiles.Union(LastEffectedTiles);
	LastEffectedNonTileLocations = EffectedNonTileLocations.Union(LastEffectedNonTileLocations);
}

/**
 * Undoes all of this affecter's effects.
 */
void UTileAffecterComponent::UndoEffect()
{
	ATile* AffectingTile = Cast<ATile>(GetOwner());

	for (TObjectPtr<UTileEffect> EachEffect : Effects)
	{
		if (IsValid(EachEffect))
		{
			EachEffect->Unaffect(LastEffectedLocations, LastEffectedTiles, LastEffectedNonTileLocations, AffectingTile);
		}
	}

	LastEffectedLocations.Empty();
	LastEffectedTiles.Empty();
	LastEffectedNonTileLocations.Empty();
}

/**
 * Gets all of the locations and tiles that will be affected.
 *
 * @param EffectedTiles - Is set to contain all of the effected tiles.
 * @param EffectedLocations - Is set to contain all of the effected locations that are not covered by tiles.
 *
 * @return All of the effected locations.
 */
TSet<FIntPoint> UTileAffecterComponent::GetEffectedTilesAndLocations(TSet<ATile*>& EffectedTiles, TSet<FIntPoint>& EffectedNonTileLocations) const
{
	TSet<FIntPoint> AffectLocations = UGridLibrary::ScaleShapeUp(ShapeLocations, Range);
	for (FIntPoint EachAffectedLocation : AffectLocations)
	{
		ATile* OverlapedTile;
		if (UGridLibrary::OverlapGridLocation(GetWorld(), EachAffectedLocation, OverlapedTile, TArray<AActor*>()))
		{
			EffectedTiles.Add(OverlapedTile);
		}
		else
		{
			EffectedNonTileLocations.Add(EachAffectedLocation);
		}
	}
	return AffectLocations;
}
/* /\ ============= /\ *\
|  /\ AAffecterTile /\  |
\* /\ ============= /\ */