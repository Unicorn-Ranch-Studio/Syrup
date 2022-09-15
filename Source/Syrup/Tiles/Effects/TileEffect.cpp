// Fill out your copyright notice in the Description page of Project Settings.


#include "TileEffect.h"
#include "EngineUtils.h"

/* \/ =========== \/ *\
|  \/ UTileEffect \/  |
\* \/ =========== \/ */
/*
 * Causes this effect.
 *
 * @param EffectedLocations - The locations to effect.
 * @param EffectedTiles - The tiles to effect.
 * @param EffectedNonTileLocations - The locations that are not covered by tiles to effect.
 * @param AffecterTile - The tile doing the affecting.
 */
void UTileEffect::Affect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile) {}

/*
 * Undoes the effect of this.
 *
 * @param EffectedLocations - The locations that were effected.
 * @param EffectedTiles - The tiles that were effected.
 * @param EffectedNonTileLocations - The locations that are not covered by tiles to that were effected.
 * @param AffecterTile - The tile doing the affecting.
 */
UFUNCTION()
void UTileEffect::Unaffect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile) {}
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */