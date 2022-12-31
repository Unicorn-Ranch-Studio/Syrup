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
UINTERFACE(BlueprintType, MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UResourceSink : public UInterface
{
    GENERATED_BODY()
};

class IResourceSink
{
    GENERATED_BODY()

public:

    /**
     * Gets the grid locations that this sink takes up.
     *
     * @return The grid locations that this sink takes up
     */
    UFUNCTION(BlueprintPure, Category = "Resources")
    virtual TSet<FIntPoint> GetAllocationLocations() const = 0;

    /**
     * Gets all the resources allocated to this.
     *
     * @return The resources allocated to this.
     */
    UFUNCTION(BlueprintPure, Category = "Resources")
    virtual TArray<UResource*> GetAllocatedResources() const = 0;

    /**
     * Undoes the effect of a resource that was sunk in this.
     *
     * @param FreedResource - The resource that was freed.
     */
    virtual void ResourceFreed(UResource* FreedResource) = 0;
};
/* /\ ============ /\ *\
|  /\ ResourceSink /\  |
\* /\ ============ /\ */