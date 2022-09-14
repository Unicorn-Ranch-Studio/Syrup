// Fill out your copyright notice in the Description page of Project Settings.


#include "TileEffect.h"
#include "EngineUtils.h"

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