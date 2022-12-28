// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceAllocationType.h"

#include "ResourceFaucet.generated.h"

class UResource;

 /* \/ ============ \/ *\
 |  \/ ResourceSink \/  |
 \* \/ ============ \/ */
/**
 * Something that can consume resources. A sink handle when resources are allocated to itself.
 */
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UResourceFaucet : public UInterface
{
    GENERATED_BODY()
};

class IResourceFaucet
{
    GENERATED_BODY()

public:
    /**
     * Gets the gets grid location locations that this faucet can allocate to.
     *
     * @return The gets grid location locations that this faucet can allocate to.
     */
    UFUNCTION(BlueprintPure, Category = "Resources")
    virtual TSet<FIntPoint> GetAllocatableLocations() const = 0;

    /**
     * Gets all the resources produced by this.
     *
     * @return The resources produced by this.
     */
    UFUNCTION(BlueprintPure, Category = "Resources")
    virtual TArray<UResource*> GetProducedResources() const = 0;
};
/* /\ ============ /\ *\
|  /\ ResourceSink /\  |
\* /\ ============ /\ */