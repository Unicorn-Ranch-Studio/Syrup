// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceSinkAmountData.h"

#include "ResourceSink.generated.h"

class UResource;

UDELEGATE(BlueprintPure)
DECLARE_DYNAMIC_DELEGATE_RetVal(TSet<FIntPoint>, FSinkLocationsDelegate);

UDELEGATE(BlueprintPure)
DECLARE_DYNAMIC_DELEGATE_RetVal(int, FSinkAmountDelegate);

DECLARE_DYNAMIC_DELEGATE_OneParam(FSinkAmountUpdateDelegate, int, NewAmount);

 /* \/ ============ \/ *\
 |  \/ ResourceSink \/  |
 \* \/ ============ \/ */
/**
 * Something that can consume resources. A sink handle when resources are allocated to itself.
 */
UCLASS(BlueprintType, CustomConstructor)
class UResourceSink : public UActorComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static UResourceSink* AddResourceSinkComponent(AActor* Owner, FResourceSinkAmountData SinkAmountData, const FSinkAmountUpdateDelegate& UpdateCallback, const FSinkLocationsDelegate& GetLocations, const FSinkAmountDelegate& GetAmount);

    /**
     * Gets all the resources allocated to this.
     *
     * @return The resources allocated to this.
     */
    UFUNCTION(BlueprintPure, Category = "Resources")
    virtual FORCEINLINE TArray<UResource*> GetAllocatedResources() const { return AllocatedResources; };
    
    /**
     * Undoes the effect of a resource that was sunk in this.
     *
     * @param FreedResource - The resource that was freed.
     */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    virtual void AllocateResource(UResource* FreedResource);

    /**
     * Undoes the effect of a resource that was sunk in this.
     *
     * @param FreedResource - The resource that was freed.
     */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    virtual void FreeResource(UResource* FreedResource);

    UPROPERTY()
    FSinkLocationsDelegate GetAllocationLocations;

    UPROPERTY()
    FSinkAmountDelegate GetAllocatedAmount;

private:
    TArray<UResource*> AllocatedResources;

    UPROPERTY()
    FSinkAmountUpdateDelegate OnAmountChanged;

    UPROPERTY()
    FResourceSinkAmountData AmountData;
};
/* /\ ============ /\ *\
|  /\ ResourceSink /\  |
\* /\ ============ /\ */