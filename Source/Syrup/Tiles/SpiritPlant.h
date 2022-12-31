// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Resources/ResourceFaucet.h"
#include "SpiritPlant.generated.h"

/**
 * 
 */
UCLASS()
class SYRUP_API ASpiritPlant : public ATile, public IResourceFaucet
{
	GENERATED_BODY()
	
    /**
     * Creates a resource.
     */
    virtual void BeginPlay() override;

    /**
     * Gets the gets grid location locations that this faucet can allocate to.
     *
     * @return The gets grid location locations that this faucet can allocate to.
     */
    virtual TSet<FIntPoint> GetAllocatableLocations() const override;

    /**
     * Gets all the resources produced by this.
     *
     * @return The resources produced by this.
     */
    virtual TArray<UResource*> GetProducedResources() const override;

    //Called when the resources produced changes.
    UPROPERTY(BlueprintAssignable)
    FFaucetUpdated OnProductionChanged;

private:
    /**
     * Destroys the freed resource.
     * 
     * @param UpdatedResource - The resource that was freed.
     */
    UFUNCTION()
    void ResourceFreed(UResource* UpdatedResource);

    /**
     * Creates a new resource.
     *
     * @param UpdatedResource - The resource that was allocated.
     */
    UFUNCTION()
    void ResourceAllocated(UResource* UpdatedResource);

    /**
     * Produces a new resource.
     */
    void ProduceResource();

    //The resources provided by this
    UPROPERTY()
    TArray<UResource*> ProducedResources = TArray<UResource*>();
};
