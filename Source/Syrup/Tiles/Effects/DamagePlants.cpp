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
		}
	}
}
/* /\ ============= /\ *\
|  /\ UDamagePlants /\  |
\* /\ ============= /\ */