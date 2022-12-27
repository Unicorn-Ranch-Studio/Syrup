// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResouceAllocationType.h"

#include "ResourceSink.generated.h"

class UResouce;

 /* \/ ============ \/ *\
 |  \/ ResourceSink \/  |
 \* \/ ============ \/ */
/**
 * Something that can consume resources. A sink handle when resources are allocated to itself.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UResourceSink : public UInterface
{
    GENERATED_BODY()
};

class IResourceSink
{
    GENERATED_BODY()

public:
    UFUNCTION()
    virtual void ResouceFreed(EResouceAllocationType FreedAllocation);

    UFUNCTION()
    virtual FVector GetAllocationLocation(EResouceAllocationType Type) const;

    UFUNCTION()
    virtual TSet<UResouce*> GetAllocatedResouces() const;
};
/* /\ ============ /\ *\
|  /\ ResourceSink /\  |
\* /\ ============ /\ */