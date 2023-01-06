// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiritPlant.h"

#include "Syrup/Systems/SyrupGameMode.h"
#include "Resources/Resource.h"

/**
 * Creates a resource.
 */
void ASpiritPlant::BeginPlay()
{
	Super::BeginPlay();

	ProduceResource();
	OnProductionChanged.Broadcast();

	ASyrupGameMode::GetTileEffectTriggerDelegate(this).AddDynamic(this, &ASpiritPlant::ReceiveEffectTrigger);
}

/**
 * Gets the gets grid location locations that this faucet can allocate to.
 *
 * @return The gets grid location locations that this faucet can allocate to.
 */
TSet<FIntPoint> ASpiritPlant::GetAllocatableLocations() const
{
	return UGridLibrary::ScaleShapeUp(GetSubTileLocations(), 1);
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
	if (bNeedsMoreResource)
	{
		bNeedsMoreResource = false;
	}
	else
	{
		ProducedResources.Remove(UpdatedResource);
		OnProductionChanged.Broadcast();
	}
}

/**
 * Creates a new resource.
 *
 * @param UpdatedResource - The resource that was allocated.
 */
void ASpiritPlant::ResourceAllocated(UResource* UpdatedResource)
{
	bNeedsMoreResource = true;
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
	bNeedsMoreResource = false;
}


/**
 * Activates the appropriate effects given the trigger.
 *
 * @param TriggerType - The of trigger that was activated.
 * @param Triggerer - The tile that triggered this effect.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
 */
void ASpiritPlant::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger)
{
	if (TriggerType == ETileEffectTriggerType::PlantsGrow && bNeedsMoreResource)
	{
		ProduceResource();
	}
}