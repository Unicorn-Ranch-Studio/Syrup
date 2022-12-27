// Fill out your copyright notice in the Description page of Project Settings.


#include "Resouce.h"

#include "ResourceSink.h"

DEFINE_LOG_CATEGORY(LogResouce);



/**
 * Allocates this resource.
 *
 * @param LinedSink - The sink to allocate this to.
 * @param Type - The kind of allocation this was.
 *
 * @return Whether this allocation was successful.
 */
bool UResouce::Allocate(TScriptInterface<IResourceSink> LinkedSink, EResouceAllocationType Type)
{
	if (!IsValid(LinkedSink.GetObject()))
	{
		UE_LOG(LogResouce, Error, TEXT("Invalid Sink"));
		return false;
	}

	if (Type == EResouceAllocationType::NotAllocated)
	{
		UE_LOG(LogResouce, Error, TEXT("Cannot allocate resource to NotAllocated. Use Free() to unallocated resources."));
		return false;
	}

	SinkAllocatedTo = LinkedSink;
	AllocationType = Type;
	return true;
}

/**
 * Unallocates this resource.
 */
void UResouce::Free()
{
	SinkAllocatedTo->ResouceFreed(AllocationType);

	SinkAllocatedTo = nullptr;
	AllocationType = EResouceAllocationType::NotAllocated;
}

/**
 * Gets whether this resource is already allocated.
 *
 * @return Whether this resource is already allocated.
 */
bool UResouce::IsAllocated() const
{
	return IsValid(SinkAllocatedTo.GetObject());
}

/**
 * Gets the sink this is allocated to.
 *
 * @param ReturnValue - The sink this is allocated to. Nullptr if unallocated.
 */
void UResouce::GetLinkedSink(TScriptInterface<IResourceSink>& ReturnValue) const
{
	ReturnValue = SinkAllocatedTo;
}


/**
 * Gets the way this has been allocated.
 *
 * @return The way this has been allocated.
 */
EResouceAllocationType UResouce::GetAllocationType() const
{
	return AllocationType;
}
