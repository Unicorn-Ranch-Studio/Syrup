// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Effects/TileEffectTrigger.h"

#include "CoreMinimal.h"
#include "Tile.h"
#include "Resources/ResourceFaucet.h"
#include "Resources/ResourceType.h"
#include "Resources/ResourceSink.h"
#include "Plant.generated.h"

class UResource;
class UApplyField;
class UPreventTrashSpawn;

DECLARE_LOG_CATEGORY_EXTERN(LogPlant, Log, All);

/* \/ ====== \/ *\
|  \/ APlant \/  |
\* \/ ====== \/ */
/**
 * A plant on the grid that grows, can take damage, and creates a protection range.
 */
UCLASS(Abstract, HideCategories = ("ActorTick", "Tile", "Replication", "Rendering", "Collision", "Actor", "Input", "HLOD", "WorldPartition", "Cooking", "DataLayers"))
class SYRUP_API APlant : public ATile, public IResourceSink, public IResourceFaucet
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

private:

	/**
	 * Binds effect triggers and initializes size.
	 */
	virtual void BeginPlay() override;

	/**
	 * Initializes Health, Range, and Shape; and sets the appropriate mesh.
	 *
	 * @param Transform - The new transform of the plant.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ------------ *\
	\* \/ Health \/ */

public:

	/**
	 * Causes this plant to take damage.
	 * 
	 * @param Amount - The number of damage points to damage this plant by.
	 * @param Cause - The tile that caused this damage.
	 * 
	 * @return Whether or not this plant was killed by the damage.
	 */
	UFUNCTION(BlueprintCallable, Category = "Plant|Health")
	bool ReceiveDamage(int Amount, ATile* Cause);

	/**
	 * Causes the visual effects of this plant taking damage
	 * 
	 * @param Amount - The number of damage points to that were caused to this plant.
	 * @param Cause - The tile that caused this damage.
	 * @param bShouldDestroy - Whether or not the plant should be destroyed once the VFX has finished.
	 */
	UFUNCTION(BlueprintNativeEvent , Category = "Plant|Health")
	void OnDamageRecived(int Amount, ATile* Cause, bool bShouldDestroy);
	FORCEINLINE void OnDamageRecived_Implementation(int Amount, ATile* Cause, bool bShouldDestroy) { if (bShouldDestroy) { Destroy(); } };
	
	/**
	 * Updates this plant to have the new amount of health.
	 * 
	 * @param NewHealth - The new health of this plant.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Plant|Health")
	void SetHealth(int NewHealth);
	void SetHealth_Implementation(int NewHealth);

	/**
	 * Gets the current health of this plant.
	 * 
	 * @return The number of damage points this plant can take before dying.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Health")
	FORCEINLINE int GetHealth() const { return Health; };

	/**
	 * Gets the amount of damage this plant has taken.
	 *
	 * @return The amount of damage this plant has taken.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Health")	
	FORCEINLINE int GetDamageTaken() const { return DamageTaken; };

	/**
	 * Gets the max health of this plant type.
	 * 
	 * @return The number of damage points a plant of this type can take before dying.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Health")
	FORCEINLINE int GetMaxHealth() const { return Cast<APlant>(GetClass()->GetDefaultObject())->Health; };
	
	/**
	 * Gets the health gained per allocated resource.
	 * 
	 * @return The health gained per allocated resource.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Health")
	FORCEINLINE int GetHealthPerResource() const { return HealthPerResource; };
	
	/**
	 * Gets the number of times health can be grown per turn.
	 * 
	 * @return The number of times health can be grown per turn.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Health")
	FORCEINLINE int GetHealthGrowthPerTurn() const { return HealthGrowthPerTurn; };
	
	/**
	 * Gets the type of resource needed to grow health.
	 * 
	 * @return The type of resource needed to grow health.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Health")
	FORCEINLINE EResourceType GetHealthGrowthResource() const { return HealthGrowthResource; };

	/**
	 * Causes the effects of this plants death.
	 */
	void Die();

protected:

	//The maximum health of this plant.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Health", Meta = (ClampMin = "1"))
	int Health = 1;

	//The health this plant starts with.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Health", Meta = (ClampMin = "1"))
	int InitialHealth = 1;

	//The health gained per resource allocation.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Health", Meta = (ClampMin = "1"))
	int HealthPerResource = 1;

	//The number of times health can be grown per turn.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Health", Meta = (ClampMin = "1"))
	int HealthGrowthPerTurn = 1;

	//The type of resource needed to grow health.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Health")
	EResourceType HealthGrowthResource = EResourceType::Any;

	//The damage taken by this plant.
	UPROPERTY(VisibleInstanceOnly)
	int DamageTaken = 0;

	/* /\ Health /\ *\
	\* ------------ */

	/* ------------ *\
	\* \/ Growth \/ */

public:
	/**
	 * Plants a plant with the given transform.
	 *
	 * @param WoldContextObject - Any object in the would to spawn the plant in.
	 * @param EnergyReserve - The variable attempt to subtract the planting cost from.
	 * @param PlantClass - The type of plant to plant.
	 * @param Transform - The location to spawn the plant at.
	 *
	 * @return Whether there was enough energy and space to plant the plant.
	 */
	UFUNCTION(BlueprintCallable, Category = "Plant|Growth", Meta = (WorldContext = "WorldContextObject"))
	static bool SowPlant(UObject* WorldContextObject, UPARAM(Ref) int& EnergyReserve, TSubclassOf<APlant> PlantClass, FTransform Transform);
	static bool SowPlant(UObject* WorldContextObject, UPARAM(Ref) int& EnergyReserve, TSubclassOf<APlant> PlantClass, FGridTransform Transform);
	static bool SowPlant(UObject* WorldContextObject, TSubclassOf<APlant> PlantClass, FTransform Transform);
	static bool SowPlant(UObject* WorldContextObject, TSubclassOf<APlant> PlantClass, FGridTransform Transform);

	/**
	 * Gets whether or not this can grow more health.
	 *
	 * @param Resource - The resource that would be allocated.	
	 * 
	 * @return Whether or not this can grow more health.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Growth|Upgrades")
	bool CanGrowHealth(UResource* Resource) const;

	/**
	 * Gets whether or not this can grow more range.
	 *
	 * @param Resource - The resource that would be allocated.	
	 *
	 * @return Whether or not this can grow more range.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Growth|Upgrades")
	bool CanGrowRange(UResource* Resource) const;

	/**
	 * Gets whether or not this can grow more production.
	 *
	 * @param Resource - The resource that would be allocated.	
	 *
	 * @return Whether or not this can grow more production.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Growth|Upgrades")
	bool CanGrowProduction(UResource* Resource) const;

	/**
	 * Causes this plant to grow more health, and allocates the given resource.
	 * 
	 * @param Resource - The resource used to grow this health.
	 * 
	 * @return Whether or not this was successful at growing more health.
	 */
	UFUNCTION(BlueprintCallable, Category = "Plant|Growth|Upgrades")
	bool GrowHealth(UResource* Resource);

	/**
	 * Causes this plant to grow more range, and allocates the given resource.
	 *
	 * @param Resource - The resource used to grow this range.
	 *
	 * @return Whether or not this was successful at growing more range.
	 */
	UFUNCTION(BlueprintCallable, Category = "Plant|Growth|Upgrades")
	bool GrowRange(UResource* Resource);

	/**
	 * Causes this plant to grow more production, and allocates the given resource.
	 *
	 * @param Resource - The resource used to grow this production.
	 *
	 * @return Whether or not this was successful at growing more production.
	 */
	UFUNCTION(BlueprintCallable, Category = "Plant|Growth|Upgrades")
	bool GrowProduction(UResource* Resource);
	
	/**
	 * Gets whether health is due to grow.
	 *
	 * @return Whether health is due to grow.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Growth|Upgrades")
	FORCEINLINE int GetNumHealthGrowing() const { return NumHealthGrowing; };

	/**
	 * Gets whether range is due to grow.
	 *
	 * @return Whether range is due to grow.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Growth|Upgrades")
	FORCEINLINE int GetNumRangeGrowing() const { return NumRangeGrowing; };

	/**
	 * Gets whether production is due to grow.
	 * 
	 * @return Whether production is due to grow.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Growth|Upgrades")
	FORCEINLINE int GetNumProductionGrowing() const { return NumProductionGrowing; };

	/**
	 * Gets cost to plant this plant type.
	 * 
	 * @return The amount of energy required to plant a plant of this type.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Growth")
	FORCEINLINE int GetPlantingCost() const { return PlantingCost; };

protected:
	/**
	 * Updates the plants so that it is 1 turn closer to fully grown, and causes the effects of being fully grown if needed.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Grow();

	/**
	 * Updates the plants so that it is 1 turn closer to fully grown, and causes the effects of being fully grown if needed.
	 */
	UFUNCTION()
	void Grow_Implementation();

	//DELETE THIS WHEN NPCs GIVE RESOURCE. 
	UPROPERTY(EditInstanceOnly)
	bool bStartGrown = false;

	//The amount of energy required to plant a plant of this type.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plant|Growth", Meta = (ClampMin = "0"))
	int PlantingCost = 1;

	//Whether or not this plant has finished being planted.
	UPROPERTY(BlueprintReadOnly, Category = "Plant|Growth")
	bool bIsFinishedPlanting = false;

private:
	//The number of health points that are currently growing.
	UPROPERTY()
	int NumHealthGrowing = 0;

	//The number of range points that are currently growing.
	UPROPERTY()
	int NumRangeGrowing = 0;

	//The number of production points that are currently growing.
	UPROPERTY()
	int NumProductionGrowing = 0;

	/* /\ Growth /\ *\
	\* ------------ */

	/* ------------ *\
	\* \/ Effect \/ */

public:
	/**
	 * Sets the range of this plant's effects.
	 *
	 * @param NewRange - The value to set the range to. Will be clamped >= 0.
	 */
	UFUNCTION(BlueprintCallable, Category = "Plant|Effect")
	void SetRange(const int NewRange);

	/**
	 * Gets the current range of this plant type's effects.
	 * 
	 * @return The scale applied to the shape of this plant type to get all effected locations of this plant type's effects.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Effect")
	FORCEINLINE int GetRange() const { return Range; };

	/**
	 * Gets the maximum range of this plant type's effects.
	 * 
	 * @return The scale applied to the shape of this plant type to get all effected locations of this plant type's effects when fully upgraded.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Effect")
	FORCEINLINE int GetMaxRange() const { return Cast<APlant>(GetClass()->GetDefaultObject())->Range; };
	
	/**
	 * Gets the range gained per allocated resource.
	 * 
	 * @return The range gained per allocated resource.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Effect")
	FORCEINLINE int GetRangePerResource() const { return RangePerResource; };
	
	/**
	 * Gets the number of times range can be grown per turn.
	 * 
	 * @return The number of times range can be grown per turn.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Effect")
	FORCEINLINE int GetRangeGrowthPerTurn() const { return RangeGrowthPerTurn; };
	
	/**
	 * Gets the type of resource needed to grow range.
	 * 
	 * @return The type of resource needed to grow range.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Effect")
	FORCEINLINE EResourceType GetRangeGrowthResource() const { return RangeGrowthResource; };

protected:

	//The scale applied to the shape of this plant type to get all effected locations of this plant type's effects.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Effect", Meta = (ClampMin = "0"))
	int Range = 1;

	//The range this plant starts with.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Effect", Meta = (ClampMin = "0"))
	int InitialRange = 1;

	//The range gained per resource allocation.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Effect", Meta = (ClampMin = "1"))
	int RangePerResource = 1;

	//The number of times range can be grown per turn.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Effect", Meta = (ClampMin = "1"))
	int RangeGrowthPerTurn = 1;

	//The type of resource needed to grow Range.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Effect")
	EResourceType RangeGrowthResource = EResourceType::Any;

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

	/**
	 * Gets the locations where the effects of this plant will apply.
	 * 
	 * @return A set of all locations where the effects of this plant will apply.
	 */
	UFUNCTION()
	TSet<FIntPoint> GetEffectLocations() const;

	/* /\ Effect /\ *\
	\* ------------ */

	/* -------------- *\
	\* \/ Resource \/ */
public:
	
	/**
	 * Updates this plant to have the new amount of production.
	 * 
	 * @param NewHealth - The new production of this plant.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Plant|Resources")
	void SetProduction(int NewProduction);
	void SetProduction_Implementation(int NewProduction);
	
	/**
	 * Gets the number of resources supplied by this plant.
	 * 
	 * @return The number of resources supplied by this plant.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Resources")
	FORCEINLINE int GetProduction() const { return Production; };

	/**
	 * Gets the maximum number of resources supplied by this plant.
	 *
	 * @return The maximum number of resources supplied by this plant.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Resources")
	FORCEINLINE int GetMaxProduction() const { return Cast<APlant>(GetClass()->GetDefaultObject())->Production; };
	
	/**
	 * Gets the production gained per allocated resource.
	 * 
	 * @return The production gained per allocated resource.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Resources")
	FORCEINLINE int GetProductionPerResource() const { return ProductionPerResource; };
	
	/**
	 * Gets the number of times production can be grown per turn.
	 * 
	 * @return The number of times production can be grown per turn.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Resources")
	FORCEINLINE int GetProductionGrowthPerTurn() const { return ProductionGrowthPerTurn; };
	
	/**
	 * Gets the type of resource needed to grow production.
	 * 
	 * @return The type of resource needed to grow production.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Resources")
	FORCEINLINE EResourceType GetProductionGrowthResource() const { return ProductionGrowthResource; };

	/**
	 * Gets the type of resource produced by this plant.
	 * 
	 * @return The type  of resource produced by this plant.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|Resources")
	FORCEINLINE EResourceType GetProductionType() const { return ProductionType; };

	/**
	 * Gets the grid locations that this sink takes up.
	 *
	 * @return The grid locations that this sink takes up
	 */
	virtual FORCEINLINE TSet<FIntPoint> GetAllocationLocations() const override { return GetSubTileLocations(); };

	/**
	 * Gets all the resources supplied by this plant.
	 * 
	 * @return The resources supplied by this plant.
	 */
	virtual FORCEINLINE TArray<UResource*> GetProducedResources() const override { return ProducedResources; };

	/**
	 * Gets the grid locations that this sink takes up.
	 *
	 * @return The grid locations that this sink takes up
	 */
	UFUNCTION(BlueprintPure, Category = "Resources")
	virtual FORCEINLINE TSet<FIntPoint> GetAllocatableLocations() const override { return GetEffectLocations(); };

	/**
	 * Gets all the resources allocated to this.
	 *
	 * @return The resources allocated to this.
	 */
	virtual FORCEINLINE TArray<UResource*> GetAllocatedResources() const override { return AllocatedResources; };

	/**
	 * Undoes the effect of a resource that was sunk in this.
	 *
	 * @param FreedResource - The resource that was freed.
	 */
	virtual void ResourceFreed(UResource* FreedResource) override;

protected:
	
	//The number of resource this plant can produce. Should be an even number.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Resources", Meta = (ClampMin = "0"))
	int Production = 2;

	//The production this plant starts with.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Resources", Meta = (ClampMin = "0"))
	int InitialProduction = 0;

	//The production gained per resource allocation.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Resources", Meta = (ClampMin = "1"))
	int ProductionPerResource = 2;

	//The number of times production can be grown per turn.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Resources", Meta = (ClampMin = "1"))
	int ProductionGrowthPerTurn = 1;

	//The type of resource needed to grow production.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Resources")
	EResourceType ProductionGrowthResource = EResourceType::Any;

	//The type of resource produced by this.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|Resources")
	EResourceType ProductionType = EResourceType::Any;

private:
	//The resources produced by this.
	UPROPERTY()
	TArray<UResource*> ProducedResources;
	
	//The resources allocated to this.
	UPROPERTY()
	TArray<UResource*> AllocatedResources;

	/* /\ Resource /\ *\
	\* -------------- */

	/* -------- *\
	\* \/ UI \/ */

public:
	
	/**
	 * Gets the name of this plant type.
	 * 
	 * @return The text to use when referring to a plant of this type.
	 */
	UFUNCTION(BlueprintPure, Category = "Plant|UI")
	FORCEINLINE FText GetDisplayName() const { return DisplayName; };

protected:

	//The text to use when referring to a plant of this type.
	UPROPERTY(EditDefaultsOnly, Category = "Plant|UI")
	FText DisplayName = FText();

	/* /\ UI /\ *\
	\* -------- */
};
/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */