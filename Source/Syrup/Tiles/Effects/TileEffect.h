// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEffectTrigger.h"
#include "TileEffect.generated.h"

class ATile;
class UTileLabel;

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
	 * Unregisters this effects labels.
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void UnregisterLabels();

	/**
	 * Registers this effects labels.
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void RegisterLabels();

	/**
	 * Tries to activate the effect
	 *
	 * @param TriggerType - The type of effects that are currently being triggered.
	 * @param Locations - The locations to effect.
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect", Meta = (AutoCreateRefTerm = "Locations"))
	void ActivateEffect(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& Locations);
	
	//The label that will be added to the location of the owner of this.
	UPROPERTY(Instanced, EditAnywhere, Category = "Effect", Meta = (AllowAbstract = "false"))
	TObjectPtr<UTileLabel> SourceLabel = nullptr;

	//The label that will be added to each of the effected locations.
	UPROPERTY(Instanced, EditAnywhere, Category = "Effect", Meta = (AllowAbstract = "false"))
	TObjectPtr<UTileLabel> EffectedLocationLabel = nullptr;

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

	/**
	 * Gets the subset of effected locations that will be labeled.
	 */
	UFUNCTION()
	virtual FORCEINLINE TSet<FIntPoint> GetLabelLocations() const { return EffectedLocations; };


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
	virtual FORCEINLINE void OnComponentDestroyed(bool bDestroyingHierarchy) override { Unaffect(); };
};
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */