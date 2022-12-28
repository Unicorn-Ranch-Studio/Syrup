// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource.h"

#include "ResourceSink.h"

DEFINE_LOG_CATEGORY(LogResource);

/**
 * Creates a resource.
 *
 * @param Faucet - The faucet that is suppling the sink. Must not be null.
 *
 * @return The resource that was created.
 */
UResource* UResource::Create(TScriptInterface<IResourceFaucet> Faucet)
{
	if (!IsValid(Faucet.GetObject())) {
		return nullptr;
	}

	UResource* NewResouce = NewObject<UResource>();
	NewResouce->FaucetCreatedby = Faucet;
	return NewResouce;
}

/**
 * Allocates this resource.
 *
 * @param LinedSink - The sink to allocate this to.
 * @param Type - The kind of allocation this was.
 *
 * @return Whether this allocation was successful.
 */
bool UResource::Allocate(TScriptInterface<IResourceSink> LinkedSink, EResourceAllocationType Type)
{
	if (!IsValid(FaucetCreatedby.GetObject()))
	{
		UE_LOG(LogResource, Error, TEXT("Invalid Faucet"));
		return false;
	}

	if (!IsValid(LinkedSink.GetObject()))
	{
		UE_LOG(LogResource, Error, TEXT("Invalid Sink"));
		return false;
	}

	if (Type == EResourceAllocationType::NotAllocated)
	{
		UE_LOG(LogResource, Error, TEXT("Cannot allocate resource to NotAllocated. Use Free() to unallocated resources."));
		return false;
	}

	if (!CanAllocateTo(LinkedSink))
	{
		return false;
	}

	SinkAllocatedTo = LinkedSink;
	AllocationType = Type;
	return true;
}

/**
 * Unallocates this resource.
 */
void UResource::Free()
{
	SinkAllocatedTo->ResourceFreed(this);

	SinkAllocatedTo = nullptr;
	AllocationType = EResourceAllocationType::NotAllocated;
}

/**
 * Gets whether this resource is already allocated.
 *
 * @return Whether this resource is already allocated.
 */
bool UResource::IsAllocated() const
{
	return IsValid(SinkAllocatedTo.GetObject());
}

/**
 * Gets whether this can be allocated to the given sink.
 *
 * @return Whether this can be allocated to the given sink.
 */
bool UResource::CanAllocateTo(TScriptInterface<IResourceSink> LinkedSink) const
{
	if (IsAllocated() || !IsValid(FaucetCreatedby.GetObject()) || !IsValid(LinkedSink.GetObject()) || LinkedSink.GetObject() == FaucetCreatedby.GetObject())
	{
		return false;
	}

	for (FIntPoint EachAllocationLocation : LinkedSink->GetAllocationLocations())
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
void UResource::GetLinkedSink(TScriptInterface<IResourceSink>& ReturnValue) const
{
	ReturnValue = SinkAllocatedTo;
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
