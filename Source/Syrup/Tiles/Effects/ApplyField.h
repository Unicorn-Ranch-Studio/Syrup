// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Syrup/MapUtilities/GroundPlane.h"

#include "CoreMinimal.h"
#include "TileEffect.h"
#include "ApplyField.generated.h"

/* \/ =========== \/ *\
|  \/ UApplyField \/  |
\* \/ =========== \/ */
/**
 * Applies a field in the affect area.
 */
UCLASS()
class SYRUP_API UApplyField : public UTileEffect
{
	GENERATED_BODY()
public:
	//The type of field to apply
	UPROPERTY(EditAnywhere)
	EFieldType FieldType = EFieldType::Protection;

	/*
	 * Causes this effect.
	 *
	 * @param EffectedLocations - The locations to effect.
	 * @param EffectedTiles - The tiles to effect.
	 * @param EffectedNonTileLocations - The locations that are not covered by tiles to effect.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	virtual void Affect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile) override;

	/*
	 * Undoes the effect of this.
	 *
	 * @param EffectedLocations - The locations that were effected.
	 * @param EffectedTiles - The tiles that were effected.
	 * @param EffectedNonTileLocations - The locations that are not covered by tiles to that were effected.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	virtual void Unaffect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile) override;

private:
	UPROPERTY()
	TSet<AGroundPlane*> GroundPlanes = TSet<AGroundPlane*>();
};
/* /\ =========== /\ *\
|  /\ UApplyField /\  |
\* /\ =========== /\ */
