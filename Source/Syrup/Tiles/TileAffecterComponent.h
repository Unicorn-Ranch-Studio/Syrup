// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Syrup/MapUtilities/GroundPlane.h"

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
UCLASS()
class SYRUP_API UTileAffecterComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * Applies all of this affecter's effects.
	 */
	UFUNCTION()
	void ApplyEffect();

	/**
	 * Undoes all of this affecter's effects.
	 */
	UFUNCTION()
	void UndoEffect();
protected:
	//Determines how big this effector is and what it will do to the tiles in its range.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAffecterData* Data = nullptr;

private:
	/**
	 * Gets all of the locations and tiles that will be affected.
	 */
	UFUNCTION()
	void GetEffectedTilesAndLocations(TSet<ATile*>& EffectedTiles, TSet<FIntPoint>& EffectedLocations) const;

	AGroundPlane* GroundPlane = nullptr;
};
/* /\ ============= /\ *\
|  /\ AAffecterTile /\  |
\* /\ ============= /\ */



/* \/ ============= \/ *\
|  \/ UAffecterData \/  |
\* \/ ============= \/ */
/**
 * Stores data about how an affecter component will act.
 */
UCLASS()
class SYRUP_API UAffecterData : public UDataAsset
{
	GENERATED_BODY()

public:
	//The number of layers to add to the shape of this affecter's effect zone.
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	int Range = 1;

	//The positions the comprise the shape of this affecter's effect zone.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<FIntPoint>	ShapeLocations = TSet<FIntPoint>();

	//The effects that this affecter will have.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<UTileEffect*> Effects = TArray<UTileEffect*>();

	//The fields that will be effected by this effect.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Bitmask, BitmaskEnum = EFieldType))
	int Fields = 0;
};
/* /\ ============= /\ *\
|  /\ UAffecterData /\  |
\* /\ ============= /\ */



/* \/ =========== \/ *\
|  \/ UTileEffect \/  |
\* \/ =========== \/ */
/**
 * A single effect that a tile effector can have.
 */
UCLASS(EditInlineNew, Abstract)
class SYRUP_API UTileEffect : public UObject
{
	GENERATED_BODY()
public:
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
	virtual void AffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile);

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
	virtual void UnaffectLocations(TSet<FIntPoint> EffectedLocations, ATile* AffecterTile);
};
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */