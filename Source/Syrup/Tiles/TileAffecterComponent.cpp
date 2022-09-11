// Fill out your copyright notice in the Description page of Project Settings.


#include "TileAffecterComponent.h"

#include "GridLibrary.h"

/* \/ ============= \/ *\
|  \/ AAffecterTile \/  |
\* \/ ============= \/ */
/**
 * Applies all of this affecter's effects.
 */
void UTileAffecterComponent::ApplyEffect()
{
	TSet<ATile*> EffectedTiles;
	TSet<FIntPoint> EffectedLocations;
	GetEffectedTilesAndLocations(EffectedTiles, EffectedLocations);

	for (UTileEffect* EachEffect : Data->Effects)
	{
		EachEffect->AffectTiles(EffectedTiles);
		EachEffect->AffectLocations(EffectedLocations);
	}
}

/**
 * Undoes all of this affecter's effects.
 */
void UTileAffecterComponent::UndoEffect()
{
	TSet<ATile*> EffectedTiles;
	TSet<FIntPoint> EffectedLocations;
	GetEffectedTilesAndLocations(EffectedTiles, EffectedLocations);

	for (UTileEffect* EachEffect : Data->Effects)
	{
		EachEffect->UnaffectTiles(EffectedTiles);
		EachEffect->UnaffectLocations(EffectedLocations);
	}
}

/**
 * Gets all of the locations and tiles that will be affected.
 */
void UTileAffecterComponent::GetEffectedTilesAndLocations(TSet<ATile*>& EffectedTiles, TSet<FIntPoint>& EffectedLocations) const
{
	TSet<FIntPoint> EffectedLocations = UGridLibrary::ScaleShapeUp(Data->ShapeLocations, Data->Range);
	for (FIntPoint EachEffectedLocation : EffectedLocations)
	{
		ATile* OverlapedTile;
		if (UGridLibrary::OverlapGridLocation(GetWorld(), EachEffectedLocation, OverlapedTile, TArray<AActor*>()))
		{
			EffectedTiles.Add(OverlapedTile);
		}
		else
		{
			EffectedLocations.Add(EachEffectedLocation);
		}
	}
}
/* /\ ============= /\ *\
|  /\ AAffecterTile /\  |
\* /\ ============= /\ */