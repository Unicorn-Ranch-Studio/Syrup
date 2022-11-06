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
	if (ensure(IsValid(CurrentLabel)))
	{
		ASyrupGameMode::UpdateTileLabel(this, CurrentLabel, Label, Location);
		CurrentLabel = Label;
		Label = Label->CreateCopy(this, FIntPoint());
	}
}

/**
 * Registers a tile label at its location.
 */
void UTileLabelComponent::BeginPlay()
{
	Super::BeginPlay();

	Location = UGridLibrary::WorldLocationToGridLocation(GetOwner()->GetActorLocation());

	Label->SourceLocations.Add(Location);
	ASyrupGameMode::RegisterTileLabel(this, Label, Location);
	CurrentLabel = Label;
	Label = Label->CreateCopy(this, FIntPoint());
}

/**
 * Handles the unregistration of the tile label.
 */
void UTileLabelComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	ASyrupGameMode::UnregisterTileLabel(this, Label, UGridLibrary::WorldLocationToGridLocation(GetOwner()->GetActorLocation()));

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

/* /\ =================== /\ *\
|  /\ UTileLabelComponent /\  |
\* /\ =================== /\ */

