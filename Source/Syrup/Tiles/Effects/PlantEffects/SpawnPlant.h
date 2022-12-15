// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Syrup/Tiles/Effects/TileEffect.h"
#include "SpawnPlant.generated.h"

class APlant;

/* \/ =========== \/ *\
|  \/ USpawnPlant \/  |
\* \/ =========== \/ */
/**
 * Spawns a plant at the effected locations.
 */
UCLASS(ClassGroup = (TileEffects), Meta = (BlueprintSpawnableComponent))
class SYRUP_API USpawnPlant : public UTileEffect
{
	GENERATED_BODY()
public:
	//The class of the plant to spawn.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta=(AllowAbstract = "false"))
	TSubclassOf<APlant> PlantClass;

protected:
	/*
	 * Causes this effect.
	 *
	 * @param Locations - The locations to effect.
	 */
	virtual void Affect(const TSet<FIntPoint>& Locations) override;
};
/* /\ =========== /\ *\
|  /\ USpawnPlant /\  |
\* /\ =========== /\ */