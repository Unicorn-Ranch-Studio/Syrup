// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpawnPlant.h"

#include "Syrup/Tiles/Plant.h"
#include "TimerManager.h"

/* \/ =========== \/ *\
|  \/ USpawnPlant \/  |
\* \/ =========== \/ */
/*
 * Causes this effect.
 *
 * @param Locations - The locations to effect.
 */
void USpawnPlant::Affect(const TSet<FIntPoint>& Locations)
{
	GetOwner()->GetWorldTimerManager().SetTimer(SpawnDelayHandle, this, &USpawnPlant::SpawnPlants, SpawnDelay, false);
	SpawnLocations = Locations.Union(SpawnLocations);
}

void USpawnPlant::SpawnPlants()
{
	for (FIntPoint EachSpawnLocation : SpawnLocations)
	{
		APlant::SowPlant(this, PlantClass, FGridTransform(EachSpawnLocation, (EGridDirection)FMath::RandHelper(3)));
	}
	SpawnLocations.Empty();
}
/* /\ =========== /\ *\
|  /\ USpawnPlant /\  |
\* /\ =========== /\ */