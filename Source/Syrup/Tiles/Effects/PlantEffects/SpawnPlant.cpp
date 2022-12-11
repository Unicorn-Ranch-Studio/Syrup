// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPlant.h"

#include "Syrup/Tiles/Plant.h"

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
	for (FIntPoint EachLocation : Locations)
	{
		APlant::SowPlant(this, PlantClass, FGridTransform(EachLocation, (EGridDirection)FMath::RandHelper(3)), InitialGrowthPercent);
	}
}
/* /\ =========== /\ *\
|  /\ USpawnPlant /\  |
\* /\ =========== /\ */