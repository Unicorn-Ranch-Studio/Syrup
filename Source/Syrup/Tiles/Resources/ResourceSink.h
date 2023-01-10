// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceSinkData.h"

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
    static UResourceSink* AddResourceSinkComponent(AActor* Owner, FResourceSinkData SinkData, const FSinkAmountUpdateDelegate& UpdateCallback, const FSinkLocationsDelegate& GetLocations, const FSinkAmountDelegate& GetAmount);
     
    /**
     * Gets the grid locations that this sink takes up.
     *
     * @return The grid locations that this sink takes up.
     */
    UFUNCTION(BlueprintPure, Category = "Resources")
    virtual TSet<FIntPoint> GetAllocationLocations() const { return AllocationLocationsGetter.Execute(); };
    
    /**
     * Gets the amount stored in this sink (not to be confused with the number of resources allocated to this).
     *
     * @return The amount stored in this sink.
     */
    UFUNCTION(BlueprintPure, Category = "Resources")
    virtual int GetAllocationAmount() const { return AllocatedAmountGetter.Execute(); };

    /**
     * Gets all the resources allocated to this.
     *
     * @return The resources allocated to this.
     */
    UFUNCTION(BlueprintPure, Category = "Resources")
    virtual FORCEINLINE TArray<UResource*> GetAllocatedResources() const { return AllocatedResources; };

    /**
     * Gets whether it is possible to allocate a resource to this.
     *
     * @param FreedResource - The resource to test.
     *
     * @return Whether it is possible to allocate the resource to this.
     */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool CanAllocateResource(UResource* FreedResource) const;

    /**
     * Causes effect of a resource that was sunk in this.
     *
     * @param FreedResource - The resource to be allocated.
     *
     * @return Whether or not the allocation was successful.
     */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    bool AllocateResource(UResource* ResourceToAllocate);

    /**
     * Undoes the effect of a resource that was sunk in this.
     *
     * @param FreedResource - The resource that was freed.
     */
    UFUNCTION(BlueprintCallable, Category = "Resources")
    void FreeResource(UResource* FreedResource);

private:
	/**
	 * Activates the appropriate effects given the trigger.
	 * 
	 * @param TriggerType - The type of trigger that was activated.
	 * @param Triggerer - The tile that triggered this effect.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
	 */
	UFUNCTION()
	void ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger);

    //The function used to get this sink's locations.
    UPROPERTY()
    FSinkLocationsDelegate AllocationLocationsGetter;

    //The function used to get this sink's amount.
    UPROPERTY()
    FSinkAmountDelegate AllocatedAmountGetter;

    //The function that causes the effects of the amount changing.
    UPROPERTY()
    FSinkAmountUpdateDelegate OnAmountChanged;

    //The resources that have been allocated to this.
    UPROPERTY()
    TArray<UResource*> AllocatedResources;

    //The data relating resource allocation to amount.
    UPROPERTY()
    FResourceSinkData Data;

    //The number of times the amount is to be incremented this turn.
    UPROPERTY()
    int IncrementsThisTurn = 0;
};
/* /\ ============ /\ *\
|  /\ ResourceSink /\  |
\* /\ ============ /\ */