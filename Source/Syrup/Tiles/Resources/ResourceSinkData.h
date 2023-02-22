// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceAllocationType.h"
#include "Syrup/Tiles/Effects/TileEffectTrigger.h"
#include "ResourceType.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateBrush.h"

#include "ResourceSinkData.generated.h"

 /* \/ ====================== \/ *\
 |  \/ ResourceSinkAmountData \/  |
 \* \/ ====================== \/ */
/**
 * Something that can consume resources. A sink handle when resources are allocated to itself.
 */
USTRUCT(BlueprintType)
struct SYRUP_API FResourceSinkData
{
    GENERATED_BODY()

    /* ---------------- *\
    \* \/ Allocation \/ */

    //The initial amount of this sink.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Allocation")
    int IntialValue = 0;
    
    //The amount gained per resource allocated.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Allocation")
    int32 IncrementPerResource = 1;

    //The type of resource needed to increment this.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Allocation")
    TArray<EResourceType> RequiredResourceTypes = TArray<EResourceType>();

    //Whether or not there is a maximum number of times this sink can be allocated to.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Allocation", Meta = (InlineEditConditionToggle))
    bool bHasMaxIncrement = true;

    //The maximum number of times this can be incramented.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Allocation", Meta = (EditCondition = "bHasMaxIncrement"))
    int MaxIncrements = 1;
    
    //Whether or not there is a maximum number of times this sink can be allocated to.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Allocation", Meta = (InlineEditConditionToggle))
    bool bHasMaxIncrementmentsPerTurn = true;

    //The number of times this can receive a resource per turn.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Allocation", Meta = (EditCondition = "bHasMaxIncrementmentsPerTurn"))
    int32 MaxIncrementmentsPerTurn = 1;

    //Whether or not incrementing the amount of this sink should be deferred.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Allocation", Meta = (InlineEditConditionToggle))
    bool bDeferredIncrement = true;

    //When to cause the increment to occur.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Allocation", Meta = (EditCondition = "bDeferredIncrement"))
    ETileEffectTriggerType IncrementTrigger = ETileEffectTriggerType::PlantsGrow;

    //The type to assign resources allocated to this. Can not be not allocated.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Allocation", Meta = (InvalidEnumValues = "NotAllocated"))
    EResourceAllocationType AllocationType = EResourceAllocationType::NotAllocated;

    /* /\ Allocation /\ *\
    \* ---------------- */

    /* -------- *\
    \* \/ UI \/ */

    //Higher values will cause this sink to be displayed last.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", Meta = (ClampMin = "0"))
    int DisplayPriority = 0;
    
    //The style of the UI used for one resource slot when a resource has been allocated to it.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FSlateBrush AllocatedStyle = FSlateBrush();

    //The style of the UI used for one resource slot when a resource has been allocated to it, but has not been activated.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FSlateBrush AllocationPendingStyle = FSlateBrush();

    //The style of the UI used for one resource slot when a is actively being allocated to it.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FButtonStyle AllocatingStyle = FButtonStyle();

    //The style of the UI used for one resource slot when a resource can be allocated to it.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FSlateBrush AllocatableStyle = FSlateBrush();

    //The style of the UI used for one resource slot when a resource can not be allocated to it.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FSlateBrush UnallocatableStyle = FSlateBrush();

    //The tool tip displayed when hovering over this sink.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FText ToolTip = FText::FromString("Please give me a tool tip.");

    /* /\ UI /\ *\
    \* -------- */
};
/* /\ ====================== /\ *\
|  /\ ResourceSinkAmountData /\  |
\* /\ ====================== /\ */