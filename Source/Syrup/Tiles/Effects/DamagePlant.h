// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileEffect.h"
#include "DamagePlant.generated.h"

/**
 * Causes damage to the plants within the specified area
 */
UCLASS(ClassGroup = (TileEffects), Meta = (BlueprintSpawnableComponent))
class SYRUP_API UDamagePlant : public UTileEffect
{
	GENERATED_BODY()

	/**
	 * Gets the current damage that this effect will cause.
	 * 
	 * @return The number of damage points this effect will cause to plants.
	 */
	UFUNCTION(BlueprintPure, Category = "Damage")
	FORCEINLINE int GetDamage() const { return Damage; };

	/**
	 * Updates the damage that this effect will deal.
	 * 
	 * @param AmountAdded - The new number of damage points this effect will cause to plants.
	 */
	UFUNCTION(BlueprintCallable, Category = "Damage")
	FORCEINLINE void SetDamage(int NewDamage) { Damage = FMath::Max(0, NewDamage); };


	/*
	 * Causes this effect.
	 *
	 * @param TriggerType - The type of effects that are currently being triggered.
	 * @param Locations - The locations to effect.
	 */
	virtual void Affect(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& Locations) override;

protected:

	//The type of field to apply
	UPROPERTY(EditDefaultsOnly, Category = "Damage", Meta = (ClampMin = "0"))
	int Damage = 1;
};
