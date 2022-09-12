// Fill out your copyright notice in the Description page of Project Settings.


#include "TileAffecterComponent.h"
#include "EngineUtils.h"

#include "GridLibrary.h"

/* \/ ============= \/ *\
|  \/ AAffecterTile \/  |
\* \/ ============= \/ */
/**
 * Applies all of this affecter's effects.
 */
void UTileAffecterComponent::ApplyEffect()
{
	ATile* AffectingTile = Cast<ATile>(GetOwner());

	TSet<ATile*> EffectedTiles;
	TSet<FIntPoint> EffectedNonTileLocations;
	TSet<FIntPoint> EffectedLocations = GetEffectedTilesAndLocations(EffectedTiles, EffectedNonTileLocations);

	for (UTileEffect* EachEffect : Effects)
	{
		if (IsValid(EachEffect))
		{
			EachEffect->AffectLocations(EffectedLocations, AffectingTile);
			EachEffect->AffectTiles(EffectedTiles, AffectingTile);
			EachEffect->AffectNonTileLocations(EffectedNonTileLocations, AffectingTile);
		}
	}
}

/**
 * Undoes all of this affecter's effects.
 */
void UTileAffecterComponent::UndoEffect()
{
	ATile* AffectingTile = Cast<ATile>(GetOwner());

	TSet<ATile*> EffectedTiles;
	TSet<FIntPoint> EffectedNonTileLocations;
	TSet<FIntPoint> EffectedLocations = GetEffectedTilesAndLocations(EffectedTiles, EffectedNonTileLocations);

	for (UTileEffect* EachEffect : Effects)
	{
		if (IsValid(EachEffect))
		{
			EachEffect->UnaffectLocations(EffectedLocations, AffectingTile);
			EachEffect->UnaffectTiles(EffectedTiles, AffectingTile);
			EachEffect->UnaffectNonTileLocations(EffectedNonTileLocations, AffectingTile);
		}
	}
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



/* \/ =========== \/ *\
|  \/ UTileEffect \/  |
\* \/ =========== \/ */
/*
 * Affects the set of all locations this effect.
 *
 * @param EffectedTiles - The locations to effect.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::AffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile) {}

/*
 * Affects the set of effected tiles with this effect.
 *
 * @param EffectedTiles - The tiles to effect.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::AffectTiles(TSet<ATile*> EffectedTiles, ATile* AffecterTile) {}

/*
 * Affects the set of locations without tiles with this effect.
 *
 * @param EffectedTiles - The locations to effect.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::AffectNonTileLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile) {}

/*
 * Undoes the affects of this on the set of a effected locations.
 *
 * @param EffectedLocations - The locations to undo the effect on.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::UnaffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile) {}

/*
 * Undoes the affects of this on the set of effected tiles.
 *
 * @param EffectedTiles - The tiles to undo the effect on.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::UnaffectTiles(TSet<ATile*> EffectedTiles, ATile* AffecterTile) {}

/*
 * Undoes the affects of this on the set of effected locations without tiles.
 *
 * @param EffectedLocations - The locations to undo the effect on.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::UnaffectNonTileLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile) {}
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */