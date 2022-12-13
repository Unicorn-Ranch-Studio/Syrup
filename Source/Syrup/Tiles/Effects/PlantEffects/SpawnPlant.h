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

	//The percent grown that the plant spawned will start at.
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "0", ClampMax = "1"))
	float InitialGrowthPercent = 0;

	//The time to wait until the spawned plant spawns.
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "0"))
	float SpawnDelay = 1;

protected:
	/*
	 * Causes this effect.
	 *
	 * @param Locations - The locations to effect.
	 */
	virtual void Affect(const TSet<FIntPoint>& Locations) override;

private:
	/**
	 * Spawns the plants after the spawn delay has elapsed.
	 */
	UFUNCTION()
	void SpawnPlants();

	//The locations to spawn plants at after the delay.
	UPROPERTY()
	TSet<FIntPoint> SpawnLocations;

	//The handle for the spawn delay timer
	UPROPERTY()
	FTimerHandle SpawnDelayHandle;
};
/* /\ =========== /\ *\
|  /\ USpawnPlant /\  |
\* /\ =========== /\ */