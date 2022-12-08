// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Trash.generated.h"

class UApplyField;
class UDamagePlants;

/* \/ ====== \/ *\
|  \/ ATrash \/  |
\* \/ ====== \/ */
/**
 * A piece of trash on the grid that can spread, cause damage, and be picked up
 */
UCLASS(Abstract, HideCategories = ("ActorTick", "Tile", "Replication", "Rendering", "Collision", "Actor", "Input", "HLOD", "WorldPartition", "Cooking", "DataLayers"))
class SYRUP_API ATrash : public ATile
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

protected:

	/**
	 * Sets up this trash after it has fallen.
	 */
	UFUNCTION(BlueprintCallable)
	void OnFinishedFalling();

private:

	/**
	 * Binds effect triggers.
	 */
	virtual void BeginPlay() override;

	/**
	 * Initializes Damage, Range, and sets the appropriate mesh.
	 *
	 * @param Transform - The new transform of the trash.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

	/* /\ Initialization /\ *\
	\* -------------------- */



	/* ------------- *\
	\* \/ Pick Up \/ */
	
public:
	/**
	 * Gets cost to pickup this piece of trash. Will fail if energy reserve does not have enough energy to pick this up.
	 * 
	 * @param EnergyReserve - The energy reserve of the thing trying to pick this up. Will have PickupCost subtracted from it.
	 * @return Whether or not this was picked up,
	 */
	UFUNCTION(BlueprintCallable, Category = "Pick Up")
	bool PickUp(UPARAM(Ref) int& EnergyReserve);

	/**
	 * Gets cost to pickup this piece of trash.
	 * 
	 * @return The number of energy points required to pickup this trash.
	 */
	UFUNCTION(BlueprintPure, Category = "Pick Up")
	FORCEINLINE int GetPickUpCost() const { return PickUpCost; };

protected:

	//The number of energy points required to pickup this trash.
	UPROPERTY(EditDefaultsOnly, Category = "Pick Up", Meta = (ClampMin = "0"))
	int PickUpCost = 1;

	/* /\ Pick Up /\ *\
	\* ------------- */



	/* ------------ *\
	\* \/ Effect \/ */
		
public:

	/**
	 * Sets the range of this trash's effects.
	 *
	 * @param NewRange - The value to set the range to. Will be clamped >= 0.
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void SetRange(const int NewRange);

	/**
	 * Gets the range of this trash's effects.
	 * 
	 * @return The scale applied to the shape of this trash to get all effected locations of this trash's effects.
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE int GetRange() const { return Range; };

	/**
	 * Sets the damage of this trash.
	 *
	 * @param NewDamage - The value to set the damage to.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Effect")
	void SetDamage(const int NewDamage);

	/**
	 * Gets the damage of this trash.
	 * 
	 * @return The damage this trash will deal to plants within its effect area every turn.
	 */
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent, Category = "Effect")
	int GetDamage() const;

	/**
	 * Gets the locations where the effects of this plant will apply.
	 * 
	 * @return A set of all locations where the effects of this plant will apply.
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	TSet<FIntPoint> GetEffectLocations() const;

protected:
	//Whether or not this trash can apply effects
	UPROPERTY()
	bool bActive = false;

	//The scale applied to the shape of this trash to get all effected locations of this trash's effects.
	UPROPERTY(EditDefaultsOnly, Category = "Effect", Meta = (ClampMin = "0"))
	int Range = 1;

private:

	/**
	 * Activates the appropriate effects given the trigger.
	 * 
	 * @param TriggerType - The of trigger that was activated.
	 * @param Triggerer - The tile that triggered this effect.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
	 */
	UFUNCTION()
	void ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger);

	/* /\ Effect /\ *\
	\* ------------ */

	/* -------- *\
	\* \/ UI \/ */

public:
	
	/**
	 * Gets the name of this trash.
	 * 
	 * @return The text to use when referring to a trash of this.
	 */
	UFUNCTION(BlueprintPure, Category = "UI")
	FORCEINLINE FText GetDisplayName() const { return DisplayName; };

protected:

	//The text to use when referring to a trash of this.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText DisplayName = FText();

	/* /\ UI /\ *\
	\* -------- */
};
/* /\ ====== /\ *\
|  /\ ATrash /\  |
\* /\ ====== /\ */