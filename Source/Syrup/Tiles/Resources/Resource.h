// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceAllocationType.h"
#include "ResourceSink.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Resource.generated.h"

class ATile;

DECLARE_LOG_CATEGORY_EXTERN(LogResource, Log, All);

/* \/ ======== \/ *\
|  \/ Resource \/  |
\* \/ ======== \/ */
/**
 * Represents a resource that can be linked to a tile to modify it.
 */
UCLASS(BlueprintType)
class SYRUP_API UResource : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	 * Allocates this resource.
	 * 
	 * @param LinedSink - The sink to allocate this to.
	 * @param Type - The kind of allocation this was.
	 * 
	 * @return Whether this allocation was successful.
	 */
	//UFUNCTION()
	bool Allocate(TScriptInterface<IResourceSink> LinkedSink, EResourceAllocationType Type);

	/**
	 * Unallocates this resource.
	 */
	//UFUNCTION()
	void Free();

	/**
	 * Gets whether this resource is already allocated.
	 * 
	 * @return Whether this resource is already allocated.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAllocated() const;

	/**
	 * Gets the sink this is allocated to.
	 *
	 * @param ReturnValue - The sink this is allocated to. Nullptr if unallocated.
	 */
	UFUNCTION(BlueprintPure)
	void GetLinkedSink(TScriptInterface<IResourceSink>& ReturnValue) const;
	

	/**
	 * Gets the way this has been allocated.
	 *
	 * @return The way this has been allocated.
	 */
	UFUNCTION(BlueprintPure)
	EResourceAllocationType GetAllocationType() const;

private:
	//The sink this is allocated to. Nullptr if unallocated.
	UPROPERTY()
	TScriptInterface<IResourceSink> SinkAllocatedTo = nullptr;

	//The way this has been allocated.
	UPROPERTY()
	EResourceAllocationType AllocationType = EResourceAllocationType::NotAllocated;
};
/* /\ ======== /\ *\
|  /\ Resource /\  |
\* /\ ======== /\ */
