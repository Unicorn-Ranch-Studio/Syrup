// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEffect.generated.h"

/* \/ =========== \/ *\
|  \/ UTileEffect \/  |
\* \/ =========== \/ */
/**
 * A single effect that a tile effector can have.
 */
UCLASS(DefaultToInstanced, abstract, editinlinenew)
class SYRUP_API UTileEffect : public UObject
{
	GENERATED_BODY()
public:
	/*
	 * Affects the set of all locations this effect.
	 *
	 * @param EffectedTiles - The locations to effect.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	UFUNCTION()
	virtual void AffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile);

	/*
	 * Affects the set of effected tiles with this effect.
	 *
	 * @param EffectedTiles - The tiles to effect.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	UFUNCTION()
	virtual void AffectTiles(TSet<ATile*> EffectedTiles, ATile* AffecterTile);

	/*
	 * Affects the set of locations without tiles with this effect.
	 *
	 * @param EffectedTiles - The locations to effect.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	UFUNCTION()
	virtual void AffectNonTileLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile);

	/*
	 * Undoes the affects of this on the set of a effected locations.
	 *
	 * @param EffectedLocations - The locations to undo the effect on.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	UFUNCTION()
	virtual void UnaffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile);

	/*
	 * Undoes the affects of this on the set of effected tiles.
	 *
	 * @param EffectedTiles - The tiles to undo the effect on.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	UFUNCTION()
	virtual void UnaffectTiles(TSet<ATile*> EffectedTiles, ATile* AffecterTile);

	/*
	 * Undoes the affects of this on the set of effected locations without tiles.
	 *
	 * @param EffectedLocations - The locations to undo the effect on.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	UFUNCTION()
	virtual void UnaffectNonTileLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile);
};
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */