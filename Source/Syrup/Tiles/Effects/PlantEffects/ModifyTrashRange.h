// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Syrup/Tiles/Effects/TileEffect.h"
#include "ModifyTrashRange.generated.h"

class ATrash;

/* \/ ================= \/ *\
|  \/ UModifyTrashRange \/  |
\* \/ ================= \/ */
/**
 * Changes the range of trash within the effect of this.
 */
UCLASS(ClassGroup = (TileEffects), Meta = (BlueprintSpawnableComponent))
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

	/*
	 * Undoes this effect.
	 *
	 * @param Locations - The locations undo the effect in.
	 */
	virtual void Unaffect(const TSet<FIntPoint>& Locations) override;

private:
	
	//All the tiles that have been effected.
	UPROPERTY()
	TSet<ATrash*> EffectedTrash = TSet<ATrash*>();
};
/* /\ ================= /\ *\
|  /\ UModifyTrashRange /\  |
\* /\ ================= /\ */