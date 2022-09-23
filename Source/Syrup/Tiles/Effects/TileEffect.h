// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEffectTrigger.h"
#include "TileEffect.generated.h"

class ATile;

/* \/ =========== \/ *\
|  \/ UTileEffect \/  |
\* \/ =========== \/ */
/**
 * A single effect that a tile effector can have.
 */
UCLASS(DefaultToInstanced, Abstract, EditInlineNew)
class SYRUP_API UTileEffect : public UObject
{
	GENERATED_BODY()
public:
	//The trigger that will cause this effect.
	UPROPERTY(EditAnywhere)
	ETileEffectTriggerType Trigger = ETileEffectTriggerType::Persistent;

	/**
	 * Causes this effect.
	 *
	 * @param EffectedLocations - The locations to effect.
	 * @param EffectedTiles - The tiles to effect.
	 * @param EffectedNonTileLocations - The locations that are not covered by tiles to effect.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	UFUNCTION()
	virtual void Affect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile);

	/**
	 * Undoes the effect of this.
	 *
	 * @param EffectedLocations - The locations that were effected.
	 * @param EffectedTiles - The tiles that were effected.
	 * @param EffectedNonTileLocations - The locations that are not covered by tiles to that were effected.
	 * @param AffecterTile - The tile doing the affecting.
	 */
	UFUNCTION()
	virtual void Unaffect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile);
};
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */