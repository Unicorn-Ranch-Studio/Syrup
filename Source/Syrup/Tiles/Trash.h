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
UCLASS(HideCategories = ("ActorTick", "Tile", "Replication", "Rendering", "Collision", "Actor", "Input", "HLOD", "WorldPartition", "Cooking", "DataLayers"))
class SYRUP_API ATrash : public ATile
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/**
	 * Creates the mesh and effect components.
	 */
	ATrash();

private:

	/**
	 * Binds effect triggers.
	 */
	virtual void BeginPlay() override;

	/**
	 * Initializes Damage, Range, and TimeUntilSpread; and sets the appropriate mesh.
	 *
	 * @param Transform - The new transform of the trash.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ----------- *\
	\* \/ Shape \/ */

	/**
	 * Gets the shape of this trash.
	 * 
	 * @return A set containing all of the relative locations of the sub-tiles making up the shape of this trash.
	 */
	UFUNCTION(BlueprintPure, Category = "Shape")
	FORCEINLINE TSet<FIntPoint> GetShape() const { return Shape; };

protected:
	/**
	 * The relative locations of all of the sub-tiles of this trash.
	 *
	 * @return The relative locations of all of the sub-tiles of this trash.
	 */
	virtual FORCEINLINE TSet<FIntPoint> GetRelativeSubTileLocations() const override { return GetShape(); };

	//A set containing all of the relative locations of the sub-tiles making up the shape of this trash.
	UPROPERTY(EditDefaultsOnly, Category = "Shape")
	TSet<FIntPoint> Shape = TSet<FIntPoint>();

	/* /\ Shape /\ *\
	\* ----------- */



	/* ------------ *\
	\* \/ Damage \/ */

	/**
	 * Gets the current damage that this trash will cause.
	 * 
	 * @return The number of damage points this trash will cause each turn to plants within its range.
	 */
	UFUNCTION(BlueprintPure, Category = "Damage")
	FORCEINLINE int GetDamage() const { return Damage; };

	/**
	 * Updates the damage that this trash will deal.
	 * 
	 * @param AmountAdded - The number of damage points to add to the current damage of this trash. May be a negative number
	 */
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void UpdateDamage(int AmountAdded = 0);

protected:

	//The number of damage points this trash will cause each turn to plants within its range.
	UPROPERTY(EditDefaultsOnly, Category = "Damage", Meta = (ClampMin = "0"))
	int Damage = 1;

	/* /\ Damage /\ *\
	\* ------------ */



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
	bool PickUp(int& EnergyReserve);

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
	 * Gets the range of this trash's effects.
	 * 
	 * @return The scale applied to the shape of this trash to get all effected locations of this trash's effects.
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE int GetRange() const { return Range; };

protected:

	//Makes the trash spread goop within the effect area.
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	UApplyField* GoopComponent;

	//Makes the trash damage plants within the effected area.
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	UDamagePlants* DamageComponent;

	//The scale applied to the shape of this trash to get all effected locations of this trash's effects.
	UPROPERTY(EditDefaultsOnly, Category = "Effect", Meta = (ClampMin = "0"))
	int Range = 1;

private:

	/**
	 * Activates the appropriate effects given the trigger.
	 * 
	 * @param TriggerType - The of trigger that was activated.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
	 */
	UFUNCTION()
	void ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& LocationsToTrigger);

	/**
	 * Gets the locations where the effects of this plant will apply.
	 * 
	 * @return A set of all locations where the effects of this plant will apply.
	 */
	UFUNCTION()
	TSet<FIntPoint> GetEffectLocations() const;

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