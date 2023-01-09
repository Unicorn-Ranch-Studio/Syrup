// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceAllocationType.h"
#include "ResourceType.h"

#include "ResourceSinkAmountData.generated.h"

 /* \/ ====================== \/ *\
 |  \/ ResourceSinkAmountData \/  |
 \* \/ ====================== \/ */
/**
 * Something that can consume resources. A sink handle when resources are allocated to itself.
 */
USTRUCT(BlueprintType)
struct SYRUP_API FResourceSinkAmountData
{
    GENERATED_BODY()

        
    //The initial amount of this sink.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int IntialValue = 0;
    
    //The amount gained per resource allocated.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 IncrementPerResource = 1;

    //The type of resource needed to increment this.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EResourceType RequiredResource = EResourceType::Any;

    //Whether or not there is a maximum number of times this sink can be allocated to.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (InlineEditConditionToggle))
    bool bHasMaxIncrement = true;

    //The maximum number of times this can be incramented.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "bHasMaxIncrement"))
    int MaxIncrements = 1;
    
    //Whether or not there is a maximum number of times this sink can be allocated to.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (InlineEditConditionToggle))
    bool bHasMaxIncrementmentsPerTurn = true;

    //The number of times this can receive a resource per turn.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "bHasMaxIncrementmentsPerTurn"))
    int32 MaxIncrementmentsPerTurn = 1;

    //The type to assign resources allocated to this. Can not be not allocated.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EResourceAllocationType AllocationType = EResourceAllocationType::NotAllocated;
};
/* /\ ====================== /\ *\
|  /\ ResourceSinkAmountData /\  |
\* /\ ====================== /\ */