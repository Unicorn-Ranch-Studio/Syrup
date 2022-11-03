// Fill out your copyright notice in the Description page of Project Settings.


#include "TileLabelComponent.h"

#include "Syrup/Systems/SyrupGameMode.h"
#include "TileLabel.h"
#include "Syrup/Tiles/GridLibrary.h"

/* \/ =================== \/ *\
|  \/ UTileLabelComponent \/  |
\* \/ =================== \/ */
/**
 * Applies any changes made to Label
 */
void UTileLabelComponent::UpdateLabel()
{
	ASyrupGameMode::UnregisterTileLabel(this, Label, Location);
	ASyrupGameMode::RegisterTileLabel(this, Label, Location);
}

/**
 * Registers a tile label at its location.
 */
void UTileLabelComponent::BeginPlay()
{
	Super::BeginPlay();

	Location = UGridLibrary::WorldLocationToGridLocation(GetComponentLocation());

	Label->SourceLocations.Add(Location);
	ASyrupGameMode::RegisterTileLabel(this, Label, Location);
}

/**
 * Handles the unregistration of the tile label.
 */
void UTileLabelComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	ASyrupGameMode::UnregisterTileLabel(this, Label, UGridLibrary::WorldLocationToGridLocation(GetComponentLocation()));

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

/* /\ =================== /\ *\
|  /\ UTileLabelComponent /\  |
\* /\ =================== /\ */

