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
 * A single effect way an actor can effect tiles.
 */
UCLASS(Abstract, HideCategories = ("Sockets", "Tags", "ComponentTick", "ComponentReplication", "Activation", "Cooking", "Components|Activation", "AssetUserData", "Collision"))
class SYRUP_API UTileEffect : public UActorComponent
{
	GENERATED_BODY()
public:
	/*
	 * Causes this effect.
	 *
	 * @param TriggerType - The type of effects that are currently being triggered.
	 * @param Locations - The locations to effect.
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect", Meta = (AutoCreateRefTerm = "Locations"))
	virtual FORCEINLINE void Affect(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& Locations) { EffectedLocations = EffectedLocations.Union(Locations); };

	/*
	 * Undoes this effect.
	 *
	 * @param TriggerType - The type of effects that are currently being undone.
	 * @param Locations - The locations to undo the effect on.
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	virtual FORCEINLINE void Unaffect(const ETileEffectTriggerType TriggerType) {};

protected:
	//The locations that have been effected by this tile already.
	UPROPERTY()
	TSet<FIntPoint> EffectedLocations = TSet<FIntPoint>();
};
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */