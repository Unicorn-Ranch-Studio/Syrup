// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEffect.h"
#include "DamagePlants.generated.h"

/* \/ ============= \/ *\
|  \/ UDamagePlants \/  |
\* \/ ============= \/ */
/**
 * Causes damage to the plants within the specified area
 */
UCLASS(ClassGroup = (TileEffects), Meta = (BlueprintSpawnableComponent))
class SYRUP_API UDamagePlants : public UTileEffect
{
	GENERATED_BODY()

public:
	/**
	 * Adds trash damage phase to triggers
	 */
	UDamagePlants();

	/**
	 * Gets the current damage that this effect will cause.
	 * 
	 * @return The number of damage points this effect will cause to plants.
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	FORCEINLINE int GetDamage() const { return Damage; };

	/**
	 * Updates the damage that this effect will deal.
	 * 
	 * @param AmountAdded - The new number of damage points this effect will cause to plants.
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	FORCEINLINE void SetDamage(int NewDamage) { Damage = FMath::Max(0, NewDamage); };

protected:

	/*
	 * Causes this effect.
	 *
	 * @param Locations - The locations to effect.
	 */
	virtual void Affect(const TSet<FIntPoint>& Locations) override;

	/**
	 * Gets the subset of the given locations that will be labeled.
	 *
	 * @param Locations - The locations that will be effected by this component
	 */
	virtual TSet<FIntPoint> GetLabelLocations(const TSet<FIntPoint>& Locations) const override;

	//The type of field to apply
	UPROPERTY(EditDefaultsOnly, Category = "Effect", Meta = (ClampMin = "0"))
	int Damage = 1;
};
/* /\ ============= /\ *\
|  /\ UDamagePlants /\  |
\* /\ ============= /\ */