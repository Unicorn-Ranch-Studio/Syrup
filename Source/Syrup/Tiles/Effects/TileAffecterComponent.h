// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Syrup/Tiles/Tile.h"
#include "TileAffecterComponent.generated.h"

class UTileEffect;

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
	UPROPERTY(EditAnywhere, AdvancedDisplay, Instanced)
	TArray<TObjectPtr<UTileEffect>> Effects = TArray<TObjectPtr<UTileEffect>>();

	/**
	 * Applies all of this affecter's effects. Note effects will only be applied once per effected location/tile.
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

private:
	//The last effected locations.
	UPROPERTY()
	TSet<FIntPoint> LastEffectedLocations = TSet<FIntPoint>();

	//The last effected tiles.
	UPROPERTY()
	TSet<ATile*> LastEffectedTiles = TSet<ATile*>();

	//The last effected non-tile locations.
	UPROPERTY()
	TSet<FIntPoint> LastEffectedNonTileLocations = TSet<FIntPoint>();
};
/* /\ ============= /\ *\
|  /\ AAffecterTile /\  |
\* /\ ============= /\ */