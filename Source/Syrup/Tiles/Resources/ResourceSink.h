// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceAllocationType.h"

#include "ResourceSink.generated.h"

class UResource;

 /* \/ ============ \/ *\
 |  \/ ResourceSink \/  |
 \* \/ ============ \/ */
/**
 * Something that can consume resources. A sink handle when resources are allocated to itself.
 */
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UResourceSink : public UInterface
{
    GENERATED_BODY()
};

class IResourceSink
{
    GENERATED_BODY()

public:
    /**
     * Undoes the effect of a resource that was sunk in this.
     *
     * @param FreedAllocation - What the freed resource's allocation was.
     */
    UFUNCTION(Category = "Resources")
    virtual void ResourceFreed(EResourceAllocationType FreedAllocation) = 0;

    /**
     * Gets the world location of the allocation.
     *
     * @return The world location of the allocation.
     */
    UFUNCTION(BlueprintPure, Category = "Resources")
    virtual FVector GetAllocationLocation(EResourceAllocationType Type) const = 0;

    /**
     * Gets all the resources allocated to this.
     *
     * @return The resources allocated to this.
     */
    UFUNCTION(BlueprintPure, Category = "Resources")
    virtual TArray<UResource*> GetAllocatedResources() const = 0;
};
/* /\ ============ /\ *\
|  /\ ResourceSink /\  |
\* /\ ============ /\ */