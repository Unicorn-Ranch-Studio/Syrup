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
void UTileAffecterComponent::ApplyEffect(TSet<FIntPoint> Locations)
{
	TSet<ATile*> EffectedTiles;
	TSet<FIntPoint> EffectedNonTileLocations;
	GetEffectedTilesAndLocations(Locations, EffectedTiles, EffectedNonTileLocations);

	ATile* AffectingTile = Cast<ATile>(GetOwner());
	EffectedTiles.Add(AffectingTile);

	EffectedNonTileLocations = Locations.Difference(LastEffectedLocations);
	EffectedTiles = EffectedTiles.Difference(LastEffectedTiles);
	EffectedNonTileLocations = EffectedNonTileLocations.Difference(LastEffectedNonTileLocations);

	for (TObjectPtr<UTileEffect> EachEffect : Effects)
	{
		if (IsValid(EachEffect))
		{
			EachEffect->Affect(Locations, EffectedTiles, EffectedNonTileLocations, AffectingTile);
		}
	}

	LastEffectedLocations = Locations.Union(LastEffectedLocations);
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
 * Gets all of the non-tile locations and tiles that will be affected by the given locations.
 *
 * @param EffectedLocations - All of the effected locations.
 * @param EffectedTiles - Is set to contain all of the effected tiles.
 * @param EffectedLocations - Is set to contain all of the effected locations that are not covered by tiles.
 */
void UTileAffecterComponent::GetEffectedTilesAndLocations(TSet<FIntPoint> EffectedLocations, TSet<ATile*>& EffectedTiles, TSet<FIntPoint>& EffectedNonTileLocations) const
{
	for (FIntPoint EachEffectedLocation : EffectedLocations)
	{
		ATile* OverlapedTile;
		if (UGridLibrary::OverlapGridLocation(GetWorld(), EachEffectedLocation, OverlapedTile, TArray<AActor*>()))
		{
			EffectedTiles.Add(OverlapedTile);
		}
		else
		{
			EffectedNonTileLocations.Add(EachEffectedLocation);
		}
	}
}
/* /\ ============= /\ *\
|  /\ AAffecterTile /\  |
\* /\ ============= /\ */