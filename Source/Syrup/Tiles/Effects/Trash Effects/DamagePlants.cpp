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
	AffectTriggers.Add(ETileEffectTriggerType::PlantSpawned);
	AffectTriggers.Add(ETileEffectTriggerType::OnActivated);
	AffectTriggers.Add(ETileEffectTriggerType::TrashActive);
	UnaffectTriggers.Add(ETileEffectTriggerType::OnDeactivated);
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
			Plant->NotifyIncomingDamage(GetDamage(), Cast<ATile>(GetOwner()));
		}
	}

	Super::Affect(Locations);
}

/**
 * Gets the subset of the given locations that will be labeled.
 * 
 * @param Locations - The locations that will be effected by this component.
 * @param bForUnregistration - Whether or not to get the label location in the case of unregistration or registration.
 */
TSet<FIntPoint> UDamagePlants::GetLabelLocations(const TSet<FIntPoint>& Locations, const bool bForUnregistration)
{
	TSet<FIntPoint> ReturnValue = TSet<FIntPoint>();

	TSet<ATile*> EffectTiles;
	UGridLibrary::OverlapShape(GetWorld(), Locations, EffectTiles, TArray<AActor*>());

	for (ATile* EachEffectedTile : EffectTiles)
	{
		APlant* Plant = Cast<APlant>(EachEffectedTile);
		if (IsValid(Plant) && (bForUnregistration ? Plant->GetSubTileLocations().Difference(Locations).Intersect(EffectedLocations).IsEmpty() : !LabeledPlants.Contains(Plant)))
		{
			ReturnValue.Add(Plant->GetGridTransform().Location);
			LabeledPlants.Add(Plant);
		}
	}

	return ReturnValue;
}

/* /\ ============= /\ *\
|  /\ UDamagePlants /\  |
\* /\ ============= /\ */