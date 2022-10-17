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
	/**
	 * Tries to activate the effect
	 *
	 * @param TriggerType - The type of effects that are currently being triggered.
	 * @param Locations - The locations to effect.
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect", Meta = (AutoCreateRefTerm = "Locations"))
	FORCEINLINE void ActivateEffect(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& Locations) 
	{ 
		if (Triggers.Contains(TriggerType))
		{
			Affect(Locations);
		}
	};

	/**
	 * Tries to undo this effect.
	 *
	 * @param TriggerType - The type of effects that are currently being undone.
	 * @param Locations - The locations to undo the effect on.
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	FORCEINLINE void UndoEffect(const ETileEffectTriggerType TriggerType) 
	{
		if (Triggers.Contains(TriggerType))
		{
			Unaffect();
		}
	};

protected:

	/**
	 * Causes the effects of this to happen, and saves the effected locations.
	 * 
	 * @param Locations - The locations that were effected.
	 */
	UFUNCTION()
	virtual FORCEINLINE void Affect(const TSet<FIntPoint>& Locations) { EffectedLocations = EffectedLocations.Union(Locations); };


	/**
	 * Undoes the effects of this.
	 */
	UFUNCTION()
	virtual FORCEINLINE void Unaffect() {};


	//The triggers that will activate this effect.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<ETileEffectTriggerType> Triggers = TSet<ETileEffectTriggerType>();

	//The locations that have been effected by this tile already.
	UPROPERTY()
	TSet<FIntPoint> EffectedLocations = TSet<FIntPoint>();

private:
	/**
	 * Called when a component is destroyed, and undoes this effect.
	 *
	 * @param	bDestroyingHierarchy  - True if the entire component hierarchy is being torn down, allows avoiding expensive operations
	 */
	virtual FORCEINLINE void OnComponentDestroyed(bool bDestroyingHierarchy) override { 
		Unaffect();
	};
};
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */