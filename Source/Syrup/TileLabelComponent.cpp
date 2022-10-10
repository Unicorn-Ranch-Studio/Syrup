// Fill out your copyright notice in the Description page of Project Settings.


#include "TileLabelComponent.h"

#include "Syrup/Systems/SyrupGameMode.h"
#include "Syrup/Tiles/GridLibrary.h"

/* \/ =================== \/ *\
|  \/ UTileLabelComponent \/  |
\* \/ =================== \/ */
/**
 * Applies any changes made to Label
 */
void UTileLabelComponent::UpdateLabel()
{
	ASyrupGameMode::UnregisterTileLabel(this, Label, UGridLibrary::WorldLocationToGridLocation(GetComponentLocation()));
	ASyrupGameMode::RegisterTileLabel(this, Label, UGridLibrary::WorldLocationToGridLocation(GetComponentLocation()));
}

/**
 * Registers a tile label at its location.
 */
void UTileLabelComponent::BeginPlay()
{
	Super::BeginPlay();

	ASyrupGameMode::RegisterTileLabel(this, Label, UGridLibrary::WorldLocationToGridLocation(GetComponentLocation()));	
}
/* /\ =================== /\ *\
|  /\ UTileLabelComponent /\  |
\* /\ =================== /\ */

