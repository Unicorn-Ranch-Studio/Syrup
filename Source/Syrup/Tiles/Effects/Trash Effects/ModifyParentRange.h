// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Syrup/Tiles/Effects/TileEffect.h"
#include "ModifyParentRange.generated.h"

/* \/ ================== \/ *\
|  \/ UModifyParentRange \/  |
\* \/ ================== \/ */
/**
 * Causes he range of the parent to change by the given amount when triggered.
 */
UCLASS(ClassGroup = (TileEffects), Meta = (BlueprintSpawnableComponent))
class SYRUP_API UModifyParentRange : public UTileEffect
{
	GENERATED_BODY()
	

public:
	//The amount to add to the range of the parent
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	int DeltaRange = 1;

protected:
	/*
	 * Causes this effect.
	 *
	 * @param Locations - The locations to effect.
	 */
	virtual void Affect(const TSet<FIntPoint>& Locations) override;
};
/* /\ ================== /\ *\
|  /\ UModifyParentRange /\  |
\* /\ ================== /\ */