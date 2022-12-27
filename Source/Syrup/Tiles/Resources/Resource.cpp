// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource.h"

#include "ResourceSink.h"

DEFINE_LOG_CATEGORY(LogResource);

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

	SinkAllocatedTo = LinkedSink;
	AllocationType = Type;
	return true;
}

/**
 * Unallocates this resource.
 */
void UResource::Free()
{
	SinkAllocatedTo->ResourceFreed(AllocationType);

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
 * Gets the sink this is allocated to.
 *
 * @param ReturnValue - The sink this is allocated to. Nullptr if unallocated.
 */
void UResource::GetLinkedSink(TScriptInterface<IResourceSink>& ReturnValue) const
{
	ReturnValue = SinkAllocatedTo;
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
