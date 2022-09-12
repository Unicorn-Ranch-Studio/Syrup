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
	if (!IsValid(GroundPlane))
	{
		for (TActorIterator<AGroundPlane> Iterator = TActorIterator<AGroundPlane>(GetWorld()); Iterator; Iterator++)
		{
			GroundPlane = *Iterator;
			break;
		}
	}

	ATile* AffectingTile = Cast<ATile>(GetOwner());

	TSet<ATile*> EffectedTiles;
	TSet<FIntPoint> EffectedLocations;
	GetEffectedTilesAndLocations(EffectedTiles, EffectedLocations);

	for (UTileEffect* EachEffect : Data->Effects)
	{
		EachEffect->AffectTiles(EffectedTiles, AffectingTile);
		EachEffect->AffectLocations(EffectedLocations, AffectingTile);
	}
	GroundPlane->ApplyField()
}

/**
 * Undoes all of this affecter's effects.
 */
void UTileAffecterComponent::UndoEffect()
{
	ATile* AffectingTile = Cast<ATile>(GetOwner());

	TSet<ATile*> EffectedTiles;
	TSet<FIntPoint> EffectedLocations;
	GetEffectedTilesAndLocations(EffectedTiles, EffectedLocations);

	for (UTileEffect* EachEffect : Data->Effects)
	{
		EachEffect->UnaffectTiles(EffectedTiles, AffectingTile);
		EachEffect->UnaffectLocations(EffectedLocations, AffectingTile);
	}
}

/**
 * Gets all of the locations and tiles that will be affected.
 */
void UTileAffecterComponent::GetEffectedTilesAndLocations(TSet<ATile*>& EffectedTiles, TSet<FIntPoint>& EffectedLocations) const
{
	TSet<FIntPoint> AffectLocationLocations = UGridLibrary::ScaleShapeUp(Data->ShapeLocations, Data->Range);
	for (FIntPoint EachAffectedLocation : AffectLocationLocations)
	{
		ATile* OverlapedTile;
		if (UGridLibrary::OverlapGridLocation(GetWorld(), EachAffectedLocation, OverlapedTile, TArray<AActor*>()))
		{
			EffectedTiles.Add(OverlapedTile);
		}
		else
		{
			EffectedLocations.Add(EachAffectedLocation);
		}
	}
}
/* /\ ============= /\ *\
|  /\ AAffecterTile /\  |
\* /\ ============= /\ */



/* \/ =========== \/ *\
|  \/ UTileEffect \/  |
\* \/ =========== \/ */
/*
 * Affects the set of effected tiles with this effect.
 *
 * @param EffectedTiles - The tiles to effect.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::AffectTiles(TSet<ATile*> EffectedTiles, ATile* AffecterTile)
{

}

/*
 * Affects the set of locations without tiles with this effect.
 *
 * @param EffectedTiles - The locations to effect.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::AffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile)
{

}

/*
 * Undoes the affects of this on the set of effected tiles.
 *
 * @param EffectedTiles - The tiles to undo the effect on.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::UnaffectTiles(TSet<ATile*> EffectedTiles, ATile* AffecterTile)
{

}

/*
 * Undoes the affects of this on the set of effected locations without tiles.
 *
 * @param EffectedLocations - The locations to undo the effect on.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::UnaffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile)
{

}
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */