// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagePlants.h"

#include "Syrup/Tiles/GridLibrary.h"
#include "Syrup/Tiles/Plant.h"

/* \/ ============= \/ *\
|  \/ UDamagePlants \/  |
\* \/ ============= \/ */
/**
 * Adds trash damage phase to triggers
 */
UDamagePlants::UDamagePlants()
{
	Triggers.Add(ETileEffectTriggerType::TrashDamage);
}

/*
 * Causes this effect.
 *
 * @param Locations - The locations to effect.
 */
void UDamagePlants::Affect(const TSet<FIntPoint>& Locations)
{
	TSet<ATile*> EffectTiles;
	UGridLibrary::OverlapShape(GetWorld(), Locations, EffectTiles, TArray<AActor*>());

	for (ATile* EachEffectedTile : EffectTiles)
	{
		APlant* Plant = Cast<APlant>(EachEffectedTile);
		if (IsValid(Plant))
		{
			Plant->ReceiveDamage(GetDamage(), Cast<ATile>(GetOwner()));
			EffectedLocations.Add(Plant->GetGridTransform().Location);
		}
	}
}

/**
 * Gets the subset of the given locations that will be labeled.
 * 
 * @param Locations - The locations that will be effected by this component
 */
TSet<FIntPoint> UDamagePlants::GetLabelLocations(const TSet<FIntPoint>& Locations) const
{
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>();

	TSet<ATile*> EffectTiles;
	UGridLibrary::OverlapShape(GetWorld(), Locations, EffectTiles, TArray<AActor*>());

	for (ATile* EachEffectedTile : EffectTiles)
	{
		APlant* Plant = Cast<APlant>(EachEffectedTile);
		if (IsValid(Plant))
		{
			ReturnValue.Add(Plant->GetGridTransform().Location);
		}
	}

	return ReturnValue;
}

/* /\ ============= /\ *\
|  /\ UDamagePlants /\  |
\* /\ ============= /\ */