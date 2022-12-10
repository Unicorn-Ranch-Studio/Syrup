// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileEffect.h"

#include "Syrup/Tiles/Tile.h"
#include "Syrup/Tiles/GridLibrary.h"
#include "Syrup/Systems/SyrupGameMode.h"
#include "Syrup/UI/Labels/TileLabel.h"

/* \/ =========== \/ *\
|  \/ UTileEffect \/  |
\* \/ =========== \/ */

/**
 * Registers this effects labels.
 * 
 * @param Locations - The locations to register labels at.
 */
void UTileEffect::RegisterLabels(const TSet<FIntPoint>& Locations)
{
	FIntPoint GridLocation = UGridLibrary::WorldLocationToGridLocation(GetOwner()->GetActorLocation());

	if (IsValid(SourceLabel) && !bSourceLabeled)
	{
		bSourceLabeled = true;
		SourceLabel->SourceLocations.Add(GridLocation);
		ASyrupGameMode::RegisterTileLabel(this, SourceLabel, GridLocation);
	}

	if (IsValid(EffectedLocationLabel))
	{
		TSet<FIntPoint> LabelLocations = GetLabelLocations(Locations, false);
		for (FIntPoint EachLabelLocation : LabelLocations)
		{
			EffectedLocationLabel->SourceLocations.Add(GridLocation);
			ASyrupGameMode::RegisterTileLabel(this, EffectedLocationLabel, EachLabelLocation);
		}
	}
}

/**
 * Unregisters this effects labels.
 * 
 * @param Locations - The locations to unregister labels at.
 */
void UTileEffect::UnregisterLabels(const TSet<FIntPoint>& Locations)
{
	FIntPoint GridLocation = UGridLibrary::WorldLocationToGridLocation(GetOwner()->GetActorLocation());

	if (IsValid(SourceLabel))
	{
		bSourceLabeled = false;
		SourceLabel->SourceLocations.Add(GridLocation);
		ASyrupGameMode::UnregisterTileLabel(this, SourceLabel, GridLocation);
	}

	if (IsValid(EffectedLocationLabel))
	{
		for (FIntPoint EachLabelLocation : GetLabelLocations(Locations, true))
		{
			EffectedLocationLabel->SourceLocations.Add(GridLocation);
			ASyrupGameMode::UnregisterTileLabel(this, EffectedLocationLabel, EachLabelLocation);
		}
	}
}

/**
 * Tries to activate the effect
 *
 * @param TriggerType - The type of effects that are currently being triggered.
 * @param Triggerer - The tile that triggered this effect.
 * @param Locations - The locations to effect.
 */
void UTileEffect::ActivateEffect(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& Locations)
{
	if (IsValid(Triggerer))
	{
		for (TSubclassOf<ATile> EachInvalidTriggererClass : InvalidTriggererClasses)
		{
			if (Triggerer->IsA(EachInvalidTriggererClass.Get()))
			{
				return;
			}
		}
	}

	if ((IsValid(SourceLabel) || IsValid(EffectedLocationLabel)))
	{
		if (TriggerType == ETileEffectTriggerType::OnActivated || TriggerType == ETileEffectTriggerType::PlantSpawned || TriggerType == ETileEffectTriggerType::TrashSpawned)
		{
			RegisterLabels(Locations);
		}
		else if (TriggerType == ETileEffectTriggerType::OnDeactivated || TriggerType == ETileEffectTriggerType::PlantKilled || TriggerType == ETileEffectTriggerType::TrashPickedUp)
		{
			UnregisterLabels(Locations);
		}
	}

	if (AffectTriggers.Contains(TriggerType))
	{
		Affect(Locations);
	}

	if (UnaffectTriggers.Contains(TriggerType))
	{
		Unaffect(Locations);
	}
}

/**
 * Called when a component is destroyed, and undoes this effect.
 *
 * @param	bDestroyingHierarchy  - True if the entire component hierarchy is being torn down, allows avoiding expensive operations
 */
void UTileEffect::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	Unaffect(EffectedLocations);
}

/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */