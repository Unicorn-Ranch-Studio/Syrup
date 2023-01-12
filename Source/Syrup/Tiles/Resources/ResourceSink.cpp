// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource.h"
#include "Syrup/Tiles/Effects/TileEffectTrigger.h"
#include "Syrup/Systems/SyrupGameMode.h"

#include "ResourceSink.h"

 /* \/ ============ \/ *\
 |  \/ ResourceSink \/  |
 \* \/ ============ \/ */

 /**
  * Adds a default resource sink to an actor. Use only in the constructor.
  *
  * @param Owner - The actor to add the sink to.
  * @param UpdateCallback - The function that should be called when the amount produced by this sink changes.
  * @param GetLocations - The function that should be called to find out what grid locations are occupied by this sink.
  * @param GetAmount - The function that should be called to find out the amount produced by this sink.
  *
  * @return The sink that was created.
  */
UResourceSink* UResourceSink::CreateDefaultResourceSinkComponent(AActor* Owner, const FSinkAmountUpdateDelegate& UpdateCallback, const FSinkLocationsDelegate& GetLocations, const FSinkAmountDelegate& GetAmount)
{
	UResourceSink* NewSink = Owner->CreateDefaultSubobject<UResourceSink>(FName(UpdateCallback.GetFunctionName().ToString() + "_Sink"));
	NewSink->OnAmountChanged = UpdateCallback;
	NewSink->AllocationLocationsGetter = GetLocations;
	NewSink->AllocatedAmountGetter = GetAmount;

	return NewSink;
}

/**
 * Adds a default resource sink to an actor. Use only in the constructor.
 *
 * @param Owner - The actor to add the sink to.
 * @param SinkData - The stats of the sink to create.
 * @param UpdateCallback - The function that should be called when the amount produced by this sink changes.
 * @param GetLocations - The function that should be called to find out what grid locations are occupied by this sink.
 * @param GetAmount - The function that should be called to find out the amount produced by this sink.
 *
 * @return The sink that was created.
 */
UResourceSink* UResourceSink::AddResourceSinkComponent(AActor* Owner, FResourceSinkData SinkData, const FSinkAmountUpdateDelegate& UpdateCallback, const FSinkLocationsDelegate& GetLocations, const FSinkAmountDelegate& GetAmount)
{
	UResourceSink* NewSink = Cast<UResourceSink>(Owner->AddComponentByClass(StaticClass(), false, FTransform(), false));;
	NewSink->Data = SinkData;
	NewSink->OnAmountChanged = UpdateCallback;
	NewSink->AllocationLocationsGetter = GetLocations;
	NewSink->AllocatedAmountGetter = GetAmount;

	ASyrupGameMode::GetTileEffectTriggerDelegate(Owner).AddDynamic(NewSink, &UResourceSink::ReceiveEffectTrigger);
	NewSink->OnAmountChanged.Execute(NewSink->Data.IntialValue);

	return NewSink;
}

/**
 * Binds appropriate delegates and sets up initial values.
 */
void UResourceSink::BeginPlay()
{
	ASyrupGameMode::GetTileEffectTriggerDelegate(this).AddDynamic(this, &UResourceSink::ReceiveEffectTrigger);
	OnAmountChanged.Execute(Data.IntialValue);
}

/**
 * Gets whether it is possible to allocate a resource to this.
 *
 * @param FreedResource - The resource to test.
 *
 * @return Whether it is possible to allocate the resource to this.
 */
bool UResourceSink::CanAllocateResource(UResource* FreedResource) const
{
	return IsValid(FreedResource) //Check validity
		&& (!Data.bHasMaxIncrement || AllocatedResources.Num() < Data.MaxIncrements) //Check max increment
		&& (!Data.bHasMaxIncrementmentsPerTurn || IncrementsThisTurn < Data.MaxIncrementmentsPerTurn) //Check per turn increment
		&& (FreedResource->GetType() == Data.RequiredResourceType || FreedResource->GetType() == EResourceType::Any || Data.RequiredResourceType == EResourceType::Any); //Check resource type
}

/**
 * Undoes the effect of a resource that was sunk in this.
 *
 * @param ResourceToAllocate - The resource that was freed.
 * 
 * @return Whether or not the allocation was successful.
 */
bool UResourceSink::AllocateResource(UResource* ResourceToAllocate)
{
	if (!CanAllocateResource(ResourceToAllocate))
	{
		return false;
	}

	ResourceToAllocate->Allocate(this, Data.AllocationType);
	if (Data.bDeferredIncrement)
	{
		IncrementsThisTurn++;
		EventOnAmountChanged.Broadcast(GetAllocationAmount());
	}
	else
	{
		int NewAmount = GetAllocationAmount() + IncrementsThisTurn * Data.IncrementPerResource;
		OnAmountChanged.Execute(NewAmount);
		EventOnAmountChanged.Broadcast(NewAmount);
	}
	return true;
}

/**
 * Undoes the effect of a resource that was sunk in this.
 *
 * @param FreedResource - The resource that was freed.
 */
void UResourceSink::FreeResource(UResource* FreedResource)
{
	FreedResource->Free();
	if (IncrementsThisTurn)
	{
		IncrementsThisTurn--;
		EventOnAmountChanged.Broadcast(GetAllocationAmount());
	}
	else
	{
		int NewAmount = GetAllocationAmount() - Data.IncrementPerResource;
		OnAmountChanged.Execute(NewAmount);
		EventOnAmountChanged.Broadcast(NewAmount);
	}
}

/**
 * Activates the appropriate effects given the trigger.
 *
 * @param TriggerType - The type of trigger that was activated.
 * @param Triggerer - The tile that triggered this effect.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
 */
void UResourceSink::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger)
{
	if (Data.bDeferredIncrement && TriggerType == Data.IncrementTrigger)
	{
		int NewAmount = GetAllocationAmount() + IncrementsThisTurn * Data.IncrementPerResource;
		IncrementsThisTurn = 0;
		OnAmountChanged.Execute(NewAmount);
		EventOnAmountChanged.Broadcast(NewAmount);
	}
}

/* /\ ============ /\ *\
|  /\ ResourceSink /\  |
\* /\ ============ /\ */