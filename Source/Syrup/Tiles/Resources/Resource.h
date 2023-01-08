// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceType.h"
#include "ResourceAllocationType.h"
#include "ResourceSink.h"
#include "ResourceFaucet.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Resource.generated.h"

class ATile;

DECLARE_LOG_CATEGORY_EXTERN(LogResource, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResourceUpdate, UResource*, UpdatedResource);

/* \/ ======== \/ *\
|  \/ Resource \/  |
\* \/ ======== \/ */
/**
 * Represents a resource that can be linked to a tile to modify it.
 */
UCLASS(BlueprintType, CustomConstructor)
class SYRUP_API UResource : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	 * Creates a resource.
	 * 
	 * @param ResourceType - The type of resource to create.
	 * @param Faucet - The faucet that is suppling the sink. Must not be null.
	 * @param Class - The class of resource to create.
	 * 
	 * @return The resource that was created.
	 */
	UFUNCTION(BlueprintCallable)
	static UResource* Create(EResourceType ResourceType, TScriptInterface<IResourceFaucet> Faucet, TSubclassOf<UResource> Class);
	static UResource* Create(EResourceType ResourceType, TScriptInterface<IResourceFaucet> Faucet);

	/**
	 * Allocates this resource.
	 * 
	 * @param LinedSink - The sink to allocate this to.
	 * @param TypeOfAllocation - The kind of allocation this was.
	 * 
	 * @return Whether this allocation was successful.
	 */
	UFUNCTION()
	bool Allocate(TScriptInterface<IResourceSink> LinkedSink, EResourceAllocationType TypeOfAllocation);

	/**
	 * Unallocates this resource.
	 */
	UFUNCTION()
	void Free();

	/**
	 * Gets whether this resource is already allocated.
	 * 
	 * @return Whether this resource is already allocated.
	 */
	UFUNCTION(BlueprintPure)
	bool IsAllocated() const;

	/**
	 * Gets whether this can be allocated to the given sink.
	 *
	 * @return Whether this can be allocated to the given sink.
	 */
	UFUNCTION(BlueprintPure)
	bool CanAllocateTo(TScriptInterface<IResourceSink> LinkedSink) const;

	/**
	 * Gets the sink this is allocated to.
	 *
	 * @param ReturnValue - The sink this is allocated to. Nullptr if unallocated.
	 */
	UFUNCTION(BlueprintPure)
	void GetLinkedSink(TScriptInterface<IResourceSink>& ReturnValue) const;

	/**
	 * Gets the faucet this is being supplied by.
	 *
	 * @param ReturnValue - The faucet this is being supplied by.
	 */
	UFUNCTION(BlueprintPure)
	void GetLinkedFaucet(TScriptInterface<IResourceFaucet>& ReturnValue) const;

	/**
	 * Gets the way this has been allocated.
	 *
	 * @return The way this has been allocated.
	 */
	UFUNCTION(BlueprintPure)
	EResourceAllocationType GetAllocationType() const;

	/**
	 * Gets type of resource that this is.
	 *
	 * @return The type of resource that this is.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE EResourceType GetType() const { return Type; };

	//Called when this is freed.
	UPROPERTY(BlueprintAssignable)
	FResourceUpdate OnFreed;

	//Called when this is allocated.
	UPROPERTY(BlueprintAssignable)
	FResourceUpdate OnAllocated;

private:
	//The sink this is allocated to. Nullptr if unallocated.
	UPROPERTY()
	TScriptInterface<IResourceSink> SinkAllocatedTo = nullptr;
	
	//The faucet this supplied by. Should never be null.
	UPROPERTY()
	TScriptInterface<IResourceFaucet> FaucetCreatedby = nullptr;

	//The way this has been allocated.
	UPROPERTY()
	EResourceAllocationType AllocationType = EResourceAllocationType::NotAllocated;

	//The type of this resource
	UPROPERTY()
	EResourceType Type = EResourceType::Any;
};
/* /\ ======== /\ *\
|  /\ Resource /\  |
\* /\ ======== /\ */
