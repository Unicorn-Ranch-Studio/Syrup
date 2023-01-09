// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource.h"

#include "ResourceSink.h"

DEFINE_LOG_CATEGORY(LogResource);

/**
 * Creates a resource.
 *
 * @param Faucet - The faucet that is suppling the sink. Must not be null.
 * @param Class - The class of resource to create.
 *
 * @return The resource that was created.
 */
UResource* UResource::Create(EResourceType ResourceType, TScriptInterface<IResourceFaucet> Faucet, TSubclassOf<UResource> Class)
{
	if (!IsValid(Faucet.GetObject())) {
		return nullptr;
	}

	UResource* NewResource = NewObject<UResource>(Class);
	NewResource->FaucetCreatedby = Faucet;
	NewResource->Type = ResourceType;
	return NewResource;
}
UResource* UResource::Create(EResourceType ResourceType, TScriptInterface<IResourceFaucet> Faucet)
{
	return Create(ResourceType, Faucet, UResource::StaticClass());
}

/**
 * Allocates this resource.
 *
 * @param LinedSink - The sink to allocate this to.
 * @param TypeOfAllocation - The kind of allocation this was.
 *
 * @return Whether this allocation was successful.
 */
bool UResource::Allocate(UResourceSink* LinkedSink, EResourceAllocationType TypeOfAllocation)
{
	if (!IsValid(FaucetCreatedby.GetObject()))
	{
		UE_LOG(LogResource, Error, TEXT("Invalid Faucet"));
		return false;
	}

	if (!IsValid(LinkedSink))
	{
		UE_LOG(LogResource, Error, TEXT("Invalid Sink"));
		return false;
	}

	if (TypeOfAllocation == EResourceAllocationType::NotAllocated)
	{
		UE_LOG(LogResource, Error, TEXT("Cannot allocate resource to NotAllocated. Use Free() to unallocated resources."));
		return false;
	}

	if (!CanAllocateTo(LinkedSink))
	{
		return false;
	}

	SinkAllocatedTo = LinkedSink;
	AllocationType = TypeOfAllocation;
	OnAllocated.Broadcast(this);
	return true;
}

/**
 * Unallocates this resource.
 */
void UResource::Free()
{
	if (IsValid(SinkAllocatedTo))
	{
		SinkAllocatedTo->FreeResource(this);
	}

	SinkAllocatedTo = nullptr;
	AllocationType = EResourceAllocationType::NotAllocated;
	OnFreed.Broadcast(this);
}

/**
 * Gets whether this resource is already allocated.
 *
 * @return Whether this resource is already allocated.
 */
bool UResource::IsAllocated() const
{
	return IsValid(SinkAllocatedTo);
}

/**
 * Gets whether this can be allocated to the given sink.
 *
 * @return Whether this can be allocated to the given sink.
 */
bool UResource::CanAllocateTo(UResourceSink* LinkedSink) const
{
	if (IsAllocated() || !IsValid(FaucetCreatedby.GetObject()) || !IsValid(LinkedSink) || LinkedSink->GetOwner() == FaucetCreatedby.GetObject())
	{
		return false;
	}

	for (FIntPoint EachAllocationLocation : LinkedSink->GetAllocationLocations.Execute())
	{
		if (FaucetCreatedby->GetAllocatableLocations().Contains(EachAllocationLocation))
		{
			return true;
		}
	}
	return false;
}


/**
 * Gets the sink this is allocated to.
 *
 * @param ReturnValue - The sink this is allocated to. Nullptr if unallocated.
 */
UResourceSink* UResource::GetLinkedSink() const
{
	return SinkAllocatedTo;
}



/**
 * Gets the faucet this is being supplied by.
 *
 * @param ReturnValue - The faucet this is being supplied by.
 */
void UResource::GetLinkedFaucet(TScriptInterface<IResourceFaucet>& ReturnValue) const
{
	ReturnValue = FaucetCreatedby;
}

/**
 * Gets the way this has been allocated.
 *
 * @return The way this has been allocated.
 */
EResourceAllocationType UResource::GetAllocationType() const
{
	return AllocationType;
}
