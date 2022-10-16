// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileEffect.h"

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

	if (IsValid(SourceLabel))
	{
		SourceLabel->SourceLocations.Add(GridLocation);
		ASyrupGameMode::RegisterTileLabel(this, SourceLabel, GridLocation);
	}

	if (IsValid(EffectedLocationLabel))
	{
		for (FIntPoint EachLabelLocation : GetLabelLocations(Locations))
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
		SourceLabel->SourceLocations.Add(GridLocation);
		ASyrupGameMode::UnregisterTileLabel(this, SourceLabel, GridLocation);
	}

	if (IsValid(EffectedLocationLabel))
	{
		for (FIntPoint EachLabelLocation : GetLabelLocations(Locations))
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
 * @param Locations - The locations to effect.
 */
void UTileEffect::ActivateEffect(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& Locations)
{ 
	if (Triggers.Contains(TriggerType))
	{
		Affect(Locations);
	}

	if ((IsValid(SourceLabel) || IsValid(EffectedLocationLabel)))
	{
		if (TriggerType == ETileEffectTriggerType::OnActivated)
		{
			RegisterLabels(Locations);
		}
		else if (TriggerType == ETileEffectTriggerType::OnDeactivated)
		{
			UnregisterLabels(Locations);
		}
	}
}

/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */