// Fill out your copyright notice in the Description page of Project Settings.


#include "TileAffecterComponent.h"

#include "EngineUtils.h"
#include "TileEffect.h"
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

	for (TObjectPtr<UTileEffect> EachEffect : Effects)
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

	for (TObjectPtr<UTileEffect> EachEffect : Effects)
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