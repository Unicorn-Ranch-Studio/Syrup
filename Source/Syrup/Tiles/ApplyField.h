// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Syrup/MapUtilities/GroundPlane.h"

#include "CoreMinimal.h"
#include "TileEffect.h"
#include "ApplyField.generated.h"

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
	 * Affects the set of all locations this effect.
	 *
	 * @param EffectedTiles - The locations to effect.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	virtual void AffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile) override;

	
	/*
	 * Affects the set of effected tiles with this effect.
	 *
	 * @param EffectedTiles - The tiles to effect.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	virtual void AffectTiles(TSet<ATile*> EffectedTiles, ATile* AffecterTile) override;

	/*
	 * Undoes the affects of this on the set of a effected locations.
	 *
	 * @param EffectedLocations - The locations to undo the effect on.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	virtual void UnaffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile) override;

	/*
	 * Undoes the affects of this on the set of effected tiles.
	 *
	 * @param EffectedTiles - The tiles to undo the effect on.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	virtual void UnaffectTiles(TSet<ATile*> EffectedTiles, ATile* AffecterTile) override;

private:
	UPROPERTY()
	AGroundPlane* GroundPlane = nullptr;
};
