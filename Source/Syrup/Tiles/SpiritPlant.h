// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Effects/TileEffectTrigger.h"
#include "Resources/ResourceType.h"

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

protected:
    //The type of resource produced by this.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EResourceType ProductionType = EResourceType::Any;

private:
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
	 * Activates the appropriate effects given the trigger.
	 * 
	 * @param TriggerType - The of trigger that was activated.
	 * @param Triggerer - The tile that triggered this effect.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
	 */
	UFUNCTION()
	void ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger);

    /**
     * Produces a new resource.
     */
    void ProduceResource();

    //Whether or not another resource needs to be produced.
    UPROPERTY()
    bool bNeedsMoreResource = false;

    //The resources provided by this
    UPROPERTY()
    TArray<UResource*> ProducedResources = TArray<UResource*>();
};
