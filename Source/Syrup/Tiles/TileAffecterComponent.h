// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Engine/DataAsset.h"
#include "TileAffecterComponent.generated.h"

/* \/ ============= \/ *\
|  \/ AAffecterTile \/  |
\* \/ ============= \/ */
/**
 * A tile that can affect other tiles in a radius around itself.
 */
UCLASS(Meta = (BlueprintSpawnableComponent))
class SYRUP_API UTileAffecterComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	//The number of layers to add to the shape of this affecter's effect zone.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (ClampMin = "0"))
	int Range = 1;

	//The positions the comprise the shape of this affecter's effect zone.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSet<FIntPoint>	ShapeLocations = TSet<FIntPoint>();

	//The effects that this affecter will have.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<UTileEffect*> Effects = TArray<UTileEffect*>();

	/**
	 * Applies all of this affecter's effects.
	 */
	UFUNCTION(BlueprintCallable)
	void ApplyEffect();

	/**
	 * Undoes all of this affecter's effects.
	 */
	UFUNCTION(BlueprintCallable)
	void UndoEffect();

	/**
	 * Gets all of the locations and tiles that will be affected.
	 * 
	 * @param EffectedTiles - Is set to contain all of the effected tiles.
	 * @param EffectedLocations - Is set to contain all of the effected locations that are not covered by tiles.
	 * 
	 * @return All of the effected locations.
	 */
	UFUNCTION(BlueprintPure)
	TSet<FIntPoint> GetEffectedTilesAndLocations(TSet<ATile*>& EffectedTiles, TSet<FIntPoint>& EffectedNonTileLocations) const;
};
/* /\ ============= /\ *\
|  /\ AAffecterTile /\  |
\* /\ ============= /\ */



/* \/ =========== \/ *\
|  \/ UTileEffect \/  |
\* \/ =========== \/ */
/**
 * A single effect that a tile effector can have.
 */
UCLASS(EditInlineNew, HideDropdown)
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