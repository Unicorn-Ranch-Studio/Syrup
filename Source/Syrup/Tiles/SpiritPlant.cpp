// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiritPlant.h"

#include "Resources/Resource.h"

/**
 * Creates a resource.
 */
void ASpiritPlant::BeginPlay()
{
	Super::BeginPlay();

	ProduceResource();
	OnProductionChanged.Broadcast();
}

/**
 * Gets the gets grid location locations that this faucet can allocate to.
 *
 * @return The gets grid location locations that this faucet can allocate to.
 */
TSet<FIntPoint> ASpiritPlant::GetAllocatableLocations() const
{
	return UGridLibrary::ScaleShapeUp(GetRelativeSubTileLocations(), 1);
}

/**
 * Gets all the resources produced by this.
 *
 * @return The resources produced by this.
 */
TArray<UResource*> ASpiritPlant::GetProducedResources() const
{
	return ProducedResources;
}

/**
 * Destroys the freed resource.
 *
 * @param UpdatedResource - The resource that was freed.
 */
void ASpiritPlant::ResourceFreed(UResource* UpdatedResource)
{
	ProducedResources.Remove(UpdatedResource);
	OnProductionChanged.Broadcast();
}

/**
 * Creates a new resource.
 *
 * @param UpdatedResource - The resource that was allocated.
 */
void ASpiritPlant::ResourceAllocated(UResource* UpdatedResource)
{
	ProduceResource();
	OnProductionChanged.Broadcast();
}

/**
 * Produces a new resource.
 */
void ASpiritPlant::ProduceResource()
{
	UResource* NewResource = UResource::Create(this);
	ProducedResources.Add(NewResource);
	NewResource->OnFreed.AddDynamic(this, &ASpiritPlant::ResourceFreed);
	NewResource->OnAllocated.AddDynamic(this, &ASpiritPlant::ResourceAllocated);
}