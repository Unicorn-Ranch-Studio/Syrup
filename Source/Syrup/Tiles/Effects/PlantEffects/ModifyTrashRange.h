// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tiles/Effects/TileEffect.h"
#include "ModifyTrashRange.generated.h"

/* \/ ================= \/ *\
|  \/ UModifyTrashRange \/  |
\* \/ ================= \/ */
/**
 * 
 */
UCLASS(ClassGroup = (TileEffects), Meta = (BlueprintSpawnableComponent)))
class SYRUP_API UModifyTrashRange : public UTileEffect
{
	GENERATED_BODY()
	
public:
	//The amount to add to the range of trash in the effected area.
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	int DeltaRange = -1;

protected:
	/**
	 * Overrides the triggers variable.
	 */
	UModifyTrashRange();

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
};
/* /\ ================= /\ *\
|  /\ UModifyTrashRange /\  |
\* /\ ================= /\ */