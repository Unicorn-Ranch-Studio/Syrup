// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Syrup/MapUtilities/GroundPlane.h"

#include "CoreMinimal.h"
#include "TileEffect.h"
#include "ApplyField.generated.h"

/* \/ =========== \/ *\
|  \/ UApplyField \/  |
\* \/ =========== \/ */
/**
 * Applies a field in the affect area.
 */
UCLASS(ClassGroup = (TileEffects), Meta = (BlueprintSpawnableComponent))
class SYRUP_API UApplyField : public UTileEffect
{
	GENERATED_BODY()
public:
	/**
	 * Overrides the triggers variable.
	 */
	UApplyField();

	/*
	 * Causes this effect.
	 *
	 * @param TriggerType - The type of effects that are currently being triggered.
	 * @param Locations - The locations to effect.
	 */
	virtual void Affect(const TSet<FIntPoint>& Locations) override;

	/*
	 * Undoes this effect.
	 *
	 * @param TriggerType - The type of effects that are currently being undone.
	 * @param Locations - The locations to undo the effect on.
	 */
	virtual void Unaffect() override;

	//The type of field to apply
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	EFieldType FieldType = EFieldType::Protection;

private:
	//All the tiles that have been effected.
	UPROPERTY()
	TSet<ATile*> EffectedTiles = TSet<ATile*>();

	//All the ground planes that have been effected.
	UPROPERTY()
	TSet<AGroundPlane*> EffectedGroundPlanes = TSet<AGroundPlane*>();
};
/* /\ =========== /\ *\
|  /\ UApplyField /\  |
\* /\ =========== /\ */
