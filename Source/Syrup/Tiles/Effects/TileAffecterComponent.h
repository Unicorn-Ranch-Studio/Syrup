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
	//The effects that this affecter will have.
	UPROPERTY(EditAnywhere, AdvancedDisplay, Instanced)
	TArray<TObjectPtr<UTileEffect>> Effects = TArray<TObjectPtr<UTileEffect>>();

	/**
	 * Applies all of this affecter's effects in the given locations. 
	 * Note: effects will only be applied once per effected location/tile.
	 * 
	 * @param Locations - The locations to apply the effects at.
	 */
	UFUNCTION(BlueprintCallable)
	void ApplyEffect(TSet<FIntPoint> Locations);

	/**
	 * Undoes all of this affecter's effects.
	 */
	UFUNCTION(BlueprintCallable)
	void UndoEffect();

	/**
	 * Gets all of the non-tile locations and tiles that will be affected by the given locations.
	 *
	 * @param EffectedLocations - All of the effected locations.
	 * @param EffectedTiles - Is set to contain all of the effected tiles.
	 * @param EffectedLocations - Is set to contain all of the effected locations that are not covered by tiles.
	 */
	UFUNCTION(BlueprintPure)
	void GetEffectedTilesAndLocations(TSet<FIntPoint> EffectedLocations, TSet<ATile*>& EffectedTiles, TSet<FIntPoint>& EffectedNonTileLocations) const;

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