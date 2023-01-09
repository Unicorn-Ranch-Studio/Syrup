// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Resources/ResourceType.h"

#include "CoreMinimal.h"
#include "Tile.h"
#include "Resources/ResourceSink.h"
#include "Trash.generated.h"

class UApplyField;
class UDamagePlants;
class UResource;

/* \/ ====== \/ *\
|  \/ ATrash \/  |
\* \/ ====== \/ */
/**
 * A piece of trash on the grid that can spread, cause damage, and be picked up
 */
UCLASS(Abstract, HideCategories = ("ActorTick", "Tile", "Replication", "Rendering", "Collision", "Actor", "Input", "HLOD", "WorldPartition", "Cooking", "DataLayers"))
class SYRUP_API ATrash : public ATile
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

protected:

	/**
	 * Sets up this trash after it has fallen.
	 */
	UFUNCTION(BlueprintCallable)
	void OnFinishedFalling();

private:

	/**
	 * Binds effect triggers.
	 */
	virtual void BeginPlay() override;

	/**
	 * Initializes Damage, Range, and sets the appropriate mesh.
	 *
	 * @param Transform - The new transform of the trash.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

	/* /\ Initialization /\ *\
	\* -------------------- */



	/* ------------- *\
	\* \/ Pick Up \/ */
	
public:
	/**
	 * Gets cost to pickup this piece of trash. Will fail if energy reserve does not have enough energy to pick this up.
	 * 
	 * @param EnergyReserve - The energy reserve of the thing trying to pick this up. Will have PickupCost subtracted from it.
	 * @return Whether or not this was picked up,
	 */
	UFUNCTION(BlueprintCallable, Category = "Trash|Pick Up")
	bool PickUp(UPARAM(Ref) int& EnergyReserve);

	/**
	 * Gets cost to pickup this piece of trash.
	 * 
	 * @return The number of energy points required to pickup this trash.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Pick Up")
	FORCEINLINE int GetPickUpCost() const { return PickUpCost; };

	/**
	 * Gets the minimum cost to pickup this piece of trash.
	 * 
	 * @return The minimum cost to pickup this piece of trash when it is fully decayed.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Pick Up")
	FORCEINLINE int GetMinPickUpCost() const { return MinPickUpCost; };	
	
	/**
	 * Gets the pickup cost lost per allocated resource.
	 * 
	 * @return The pickup cost lost per allocated resource.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Pick Up")
	FORCEINLINE int GetPickUpCostPerResource() const { return PickUpCostPerResource; };
	
	/**
	 * Gets the number of times pickup cost can be decayed per turn.
	 * 
	 * @return The number of times pickup cost can be decayed per turn.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Pick Up")
	FORCEINLINE int GetPickUpCostDecayPerTurn() const { return PickUpCostDecayPerTurn; };
	
	/**
	 * Gets the type of resource needed to decay pickup cost.
	 * 
	 * @return The type of resource needed to decay pickup cost.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Pick Up")
	FORCEINLINE EResourceType GetPickUpCostDecayResource() const { return PickUpCostDecayResource; };

protected:

	//The number of energy points required to pickup this trash.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Pick Up", Meta = (ClampMin = "0"))
	int PickUpCost = 1;
	
	//The minimum number of energy points required to pickup this trash after it has been fully decayed.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Pick Up", Meta = (ClampMin = "0"))
	int MinPickUpCost = 1;
	
	//The number of energy points removed from the trash per decay.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Pick Up", Meta = (ClampMin = "1"))
	int PickUpCostPerResource = 2;
	
	//The number of times pickup cost can be decayed per turn.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Pick Up", Meta = (ClampMin = "1"))
	int PickUpCostDecayPerTurn = 1;

	//The type of resource needed to decay pickup cost.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Pick Up")
	EResourceType PickUpCostDecayResource = EResourceType::Any;


	/* /\ Pick Up /\ *\
	\* ------------- */



	/* ------------ *\
	\* \/ Effect \/ */
		
public:

	/**
	 * Sets the range of this trash's effects.
	 *
	 * @param NewRange - The value to set the range to. Will be clamped >= 0.
	 */
	UFUNCTION(BlueprintCallable, Category = "Trash|Effect")
	void SetRange(const int NewRange);

	/**
	 * Gets the range of this trash's effects.
	 * 
	 * @return The scale applied to the shape of this trash to get all effected locations of this trash's effects.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Effect")
	FORCEINLINE int GetRange() const { return Range; };

	/**
	 * Gets the minimum range of this trash.
	 * 
	 * @return The minimum range of this trash when it is fully decayed.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Effect")
	FORCEINLINE int GetMinRange() const { return MinRange; };	
	
	/**
	 * Gets the range lost per allocated resource.
	 * 
	 * @return The range lost  per allocated resource.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Effect")
	FORCEINLINE int GetRangePerResource() const { return RangePerResource; };
	
	/**
	 * Gets the number of times range can be decayed per turn.
	 * 
	 * @return The number of times range can be decayed per turn.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Effect")
	FORCEINLINE int GetRangeDecayPerTurn() const { return RangeDecayPerTurn; };
	
	/**
	 * Gets the type of resource needed to decay range.
	 * 
	 * @return The type of resource needed to decay range.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Effect")
	FORCEINLINE EResourceType GetRangeDecayResource() const { return RangeDecayResource; };

	/**
	 * Sets the damage of this trash.
	 *
	 * @param NewDamage - The value to set the damage to.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Trash|Effect")
	void SetDamage(const int NewDamage);

	/**
	 * Gets the damage of this trash.
	 * 
	 * @return The damage this trash will deal to plants within its effect area every turn.
	 */
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent, Category = "Trash|Effect")
	int GetDamage() const;

	/**
	 * Gets the minimum damage of this trash.
	 * 
	 * @return The minimum damage of this trash when it is fully decayed.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Effect")
	FORCEINLINE int GetMinDamage() const { return MinDamage; };	
	
	/**
	 * Gets the damage lost per allocated resource.
	 * 
	 * @return The damage lost  per allocated resource.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Effect")
	FORCEINLINE int GetDamagePerResource() const { return DamagePerResource; };
	
	/**
	 * Gets the number of times damage can be decayed per turn.
	 * 
	 * @return The number of times damage can be decayed per turn.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Effect")
	FORCEINLINE int GetDamageDecayPerTurn() const { return DamageDecayPerTurn; };
	
	/**
	 * Gets the type of resource needed to decay damage.
	 * 
	 * @return The type of resource needed to decay damage.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Effect")
	FORCEINLINE EResourceType GetDamageDecayResource() const { return DamageDecayResource; };

	/**
	 * Gets the locations where the effects of this plant will apply.
	 * 
	 * @return A set of all locations where the effects of this plant will apply.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Effect")
	TSet<FIntPoint> GetEffectLocations() const;

protected:
	//Whether or not this trash can apply effects
	UPROPERTY()
	bool bActive = false;

	//The scale applied to the shape of this trash to get all effected locations of this trash's effects.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Effect", Meta = (ClampMin = "0"))
	int Range = 1;
	
	//The minimum range after this has been fully decayed.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Effect", Meta = (ClampMin = "0"))
	int MinRange = 1;
	
	//The number of range removed from the trash per decay.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Effect", Meta = (ClampMin = "1"))
	int RangePerResource = 1;

	//The amount of range can be decayed per turn.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Effect", Meta = (ClampMin = "1"))
	int RangeDecayPerTurn = 1;
	
	//The type of resource needed to decay range.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Effect")
	EResourceType RangeDecayResource = EResourceType::Any;

	//The number of hp points dealt to trash in range.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trash|Damage", Meta = (ClampMin = "0"))
	int Damage = 1;
	
	//The minimum damage after this has been fully decayed.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Damage", Meta = (ClampMin = "0"))
	int MinDamage = 1;
	
	//The number of damage removed from the trash per decay.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Damage", Meta = (ClampMin = "1"))
	int DamagePerResource = 1;
	
	//The type of resource needed to decay damage.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Effect")
	EResourceType DamageDecayResource = EResourceType::Any;
	
	//The amount of damage can be decayed per turn.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|Damage", Meta = (ClampMin = "1"))
	int DamageDecayPerTurn = 1;

private:

	/**
	 * Activates the appropriate effects given the trigger.
	 * 
	 * @param TriggerType - The of trigger that was activated.
	 * @param Triggerer - The tile that triggered this effect.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
	 */
	UFUNCTION()
	void ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger);

	/* /\ Effect /\ *\
	\* ------------ */

	
	
	/* -------------- *\
	\* \/ Resource \/ */

public:
	/**
	 * Gets whether or not this can lose more damage.
	 *
	 * @param Resource - The resource that would be allocated.
	 * 
	 * @return Whether or not this can lose more damage.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Resource|Upgrades")
	bool CanDecayDamage(UResource* Resource) const;

	/**
	 * Gets whether or not this can lose more range.
	 *
	 * @param Resource - The resource that would be allocated.	
	 *
	 * @return Whether or not this can lose more range.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Resource|Upgrades")
	bool CanDecayRange(UResource* Resource) const;

	/**
	 * Gets whether or not this can lose more pickup cost.
	 *
	 * @param Resource - The resource that would be allocated.	
	 *
	 * @return Whether or not this can lose more pickup cost.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Resource|Upgrades")
	bool CanDecayPickupCost(UResource* Resource) const;

	/**
	 * Causes this trash to lose damage, and allocates the given resource.
	 * 
	 * @param Resource - The resource used to decay this damage.
	 * 
	 * @return Whether or not this was successful at decaying more damage.
	 */
	UFUNCTION(BlueprintCallable, Category = "Trash|Resource|Upgrades")
	bool DecayDamage(UResource* Resource);

	/**
	 * Causes this trash to lose range, and allocates the given resource.
	 *
	 * @param Resource - The resource used to decay this range.
	 *
	 * @return Whether or not this was successful at decaying more range.
	 */
	UFUNCTION(BlueprintCallable, Category = "Trash|Resource|Upgrades")
	bool DecayRange(UResource* Resource);

	/**
	 * Causes this trash to lose pickup cost, and allocates the given resource.
	 *
	 * @param Resource - The resource used to decay this pickup cost.
	 *
	 * @return Whether or not this was successful at decaying more pickup cost.
	 */
	UFUNCTION(BlueprintCallable, Category = "Trash|Resource|Upgrades")
	bool DecayPickupCost(UResource* Resource);
	
	/**
	 * Gets whether damage is due to decay.
	 *
	 * @return Whether damage is due to decay.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Resource|Upgrades")
	FORCEINLINE int GetNumDamageDecaying() const { return NumDamageDecaying; };

	/**
	 * Gets whether range is due to decay.
	 *
	 * @return Whether range is due to decay.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Resource|Upgrades")
	FORCEINLINE int GetNumRangeDecaying() const { return NumRangeDecaying; };

	/**
	 * Gets whether pickup cost is due to decay.
	 * 
	 * @return Whether pickup cost is due to decay.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|Resource|Upgrades")
	FORCEINLINE int GetNumPickupCostDecaying() const { return NumPickupCostDecaying; };

	/**
     * Gets the grid locations that this sink takes up.
     *
     * @return The grid locations that this sink takes up///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     */
	virtual FORCEINLINE TSet<FIntPoint> GetAllocationLocations() const { return GetSubTileLocations(); };

    /**
     * Gets all the resources allocated to this.
     *
     * @return The resources allocated to this.///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     */
	virtual FORCEINLINE TArray<UResource*> GetAllocatedResources() const { return AllocatedResources; };

    /**
     * Undoes the effect of a resource that was sunk in this.
     *
     * @param FreedResource - The resource that was freed.///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     */
    virtual void ResourceFreed(UResource* FreedResource);

protected:
	/**
	 * Causes the effects of decaying.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Decay();
	void Decay_Implementation();

private:
	//Whether or not damage is in the process of decaying.
	UPROPERTY()
	int NumDamageDecaying = 0;

	//Whether or not range is in the process of decaying.
	UPROPERTY()
	int NumRangeDecaying = 0;
	
	//Whether or not pickup cost is in the process of decaying.
	UPROPERTY()
	int NumPickupCostDecaying = 0;

	//The resources that have been allocated to this.
	UPROPERTY()
	TArray<UResource*> AllocatedResources;

	/* /\ Resource /\ *\
	\* -------------- */



	/* -------- *\
	\* \/ UI \/ */

public:
	
	/**
	 * Gets the name of this trash.
	 * 
	 * @return The text to use when referring to a trash of this.
	 */
	UFUNCTION(BlueprintPure, Category = "Trash|UI")
	FORCEINLINE FText GetDisplayName() const { return DisplayName; };

protected:

	//The text to use when referring to a trash of this.
	UPROPERTY(EditDefaultsOnly, Category = "Trash|UI")
	FText DisplayName = FText();

	/* /\ UI /\ *\
	\* -------- */
};
/* /\ ====== /\ *\
|  /\ ATrash /\  |
\* /\ ====== /\ */