// Fill out your copyright notice in the Description page of Project Settings.


#include "Trash.h"

#include "Syrup/Systems/SyrupGameMode.h"
#include "Effects/TileEffect.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Resources/Resource.h"

/* \/ ====== \/ *\
|  \/ ATrash \/  |
\* \/ ====== \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Sets up this trash after it has fallen.
 */
void ATrash::OnFinishedFalling()
{ 
	bActive = true;
	ReceiveEffectTrigger(ETileEffectTriggerType::OnActivated, nullptr, TSet<FIntPoint>());
}

/**
 * Binds effect triggers.
 */
void ATrash::BeginPlay()
{
	Super::BeginPlay();

	ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::TrashSpawned, this, GetSubTileLocations());
	ASyrupGameMode::GetTileEffectTriggerDelegate(this).AddDynamic(this, &ATrash::ReceiveEffectTrigger);
}

/**
 * Initializes Damage, Range, and sets the appropriate mesh.
 *
 * @param Transform - The new transform of the trash.
 */
void ATrash::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SubtileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SetDamage(Damage);
	RelativeSubTileLocations.Add(FIntPoint::ZeroValue);
}

/* /\ Initialization /\ *\
\* -------------------- */



/* ------------- *\
\* \/ Pick Up \/ */

/**
 * Gets cost to pickup this piece of trash. Will fail if energy reserve does not have enough energy to pick this up.
 *
 * @param EnergyReserve - The energy reserve of the thing trying to pick this up. Will have PickupCost subtracted from it.
 * @return Whether or not this was picked up,
 */
bool ATrash::PickUp(int& EnergyReserve)
{
	if (EnergyReserve >= PickUpCost)
	{
		EnergyReserve -= PickUpCost;
		ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::TrashPickedUp, this, GetSubTileLocations());
		ReceiveEffectTrigger(ETileEffectTriggerType::OnDeactivated, nullptr, TSet<FIntPoint>());
		Destroy();
		return true;
	}

	return false;
}

/* /\ Pick Up /\ *\
\* ------------- */



/* ------------ *\
\* \/ Effect \/ */

/**
 * Sets the range of this trash's effects.
 *
 * @param NewRange - The value to set the range to. Will be clamped >= 0.
 */
void ATrash::SetRange(const int NewRange)
{
	TSet<FIntPoint> OldEffectLocations = GetEffectLocations();
	TSet<FIntPoint> NewEffectLocations = UGridLibrary::ScaleShapeUp(GetSubTileLocations(), FMath::Max(0, NewRange));

	TSet<FIntPoint> DeactivatedLocations = OldEffectLocations.Difference(NewEffectLocations);
	if (!DeactivatedLocations.IsEmpty())
	{
		ReceiveEffectTrigger(ETileEffectTriggerType::OnDeactivated, nullptr, DeactivatedLocations);
	}

	Range = FMath::Max(0, NewRange);
	TSet<FIntPoint> ActivatedLocations = NewEffectLocations.Difference(OldEffectLocations);
	if (!ActivatedLocations.IsEmpty())
	{
		ReceiveEffectTrigger(ETileEffectTriggerType::OnActivated, nullptr, ActivatedLocations);
	}
}

/**
 * Activates the appropriate effects given the trigger.
 *
 * @param TriggerType - The type of trigger that was activated.
 * @param Triggerer - The tile that triggered this effect.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
 */
void ATrash::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger)
{
	if (bActive)
	{
		if (TriggerType == ETileEffectTriggerType::PlantsGrow)
		{
			Decay();
		}

		TSet<FIntPoint> EffectedLocations = GetEffectLocations();
		TSet<FIntPoint> TriggeredLocations = LocationsToTrigger.IsEmpty() ? EffectedLocations : LocationsToTrigger.Intersect(EffectedLocations);

		if (!TriggeredLocations.IsEmpty())
		{
			TInlineComponentArray<UActorComponent*> Components = TInlineComponentArray<UActorComponent*>();
			GetComponents(UTileEffect::StaticClass(), Components);
			for (UActorComponent* EachComponent : Components)
			{
				Cast<UTileEffect>(EachComponent)->ActivateEffect(TriggerType, Triggerer, TriggeredLocations);
			}
		}
	}
}

/**
 * Gets the locations where the effects of this trash will apply.
 *
 * @return A set of all locations where the effects of this trash will apply.
 */
TSet<FIntPoint> ATrash::GetEffectLocations() const
{
	return UGridLibrary::ScaleShapeUp(GetSubTileLocations(), Range);
}

/* /\ Effect /\ *\
\* ------------ */


/* -------------- *\
\* \/ Resource \/ */

/**
 * Gets whether or not this can lose more damage.
 *
 * @param Resource - The resource that would be allocated.
 *
 * @return Whether or not this can lose more damage.
 */
bool ATrash::CanDecayDamage(UResource* Resource) const
{
	return IsValid(Resource)
		&& (DamageDecayResource == Resource->GetType() || DamageDecayResource == EResourceType::Any || Resource->GetType() == EResourceType::Any)
		&& GetDamage() - NumDamageDecaying > MinDamage 
		&& NumDamageDecaying < DamageDecayPerTurn * DamagePerResource;
}

/**
 * Gets whether or not this can lose more range.
 *
 * @param Resource - The resource that would be allocated.
 *
 * @return Whether or not this can lose more range.
 */
bool ATrash::CanDecayRange(UResource* Resource) const
{
	return IsValid(Resource)
		&& (RangeDecayResource == Resource->GetType() || RangeDecayResource == EResourceType::Any || Resource->GetType() == EResourceType::Any)
		&& GetRange() - NumRangeDecaying > MinRange 
		&& NumRangeDecaying < RangeDecayPerTurn * RangePerResource;
}

/**
 * Gets whether or not this can lose more pickup cost.
 *
 * @param Resource - The resource that would be allocated.
 *
 * @return Whether or not this can lose more pickup cost.
 */
bool ATrash::CanDecayPickupCost(UResource* Resource) const
{
	return IsValid(Resource)
		&& (PickUpCostDecayResource == Resource->GetType() || PickUpCostDecayResource == EResourceType::Any || Resource->GetType() == EResourceType::Any)
		&& GetPickUpCost() - NumPickupCostDecaying > MinPickUpCost 
		&& NumPickupCostDecaying < PickUpCostDecayPerTurn * PickUpCostPerResource;
}

/**
 * Causes this trash to lose damage, and allocates the given resource.
 *
 * @param Resource - The resource used to decay this damage.
 *
 * @return Whether or not this was successful at decaying more damage.
 */
bool ATrash::DecayDamage(UResource* Resource)
{
	if (CanDecayDamage(Resource))
	{
		NumDamageDecaying = FMath::Min(NumDamageDecaying + DamagePerResource, Damage - MinDamage);
		//Resource->Allocate(this, EResourceAllocationType::TrashDamage);
		return true;
	}
	return false;
}

/**
 * Causes this trash to lose range, and allocates the given resource.
 *
 * @param Resource - The resource used to decay this range.
 *
 * @return Whether or not this was successful at decaying more range.
 */
bool ATrash::DecayRange(UResource* Resource)
{
	if (CanDecayRange(Resource))
	{
		NumRangeDecaying = FMath::Min(NumRangeDecaying + RangePerResource, Range - MinRange);
		//Resource->Allocate(this, EResourceAllocationType::TrashRange);
		return true;
	}
	return false;
}

/**
 * Causes this trash to lose pickup cost, and allocates the given resource.
 *
 * @param Resource - The resource used to decay this pickup cost.
 *
 * @return Whether or not this was successful at decaying more pickup cost.
 */
bool ATrash::DecayPickupCost(UResource* Resource)
{
	if (CanDecayPickupCost(Resource))
	{
		NumPickupCostDecaying = FMath::Min(NumPickupCostDecaying + PickUpCostPerResource, PickUpCost - MinPickUpCost);
		//Resource->Allocate(this, EResourceAllocationType::TrashCost);
		return true;
	}
	return false;
}

/**
 * Causes the effects of decaying.
 */
void ATrash::Decay_Implementation()
{
	if (NumDamageDecaying)
	{
		SetDamage(GetDamage() - NumDamageDecaying);
		NumDamageDecaying = 0;
	}
	if (NumRangeDecaying)
	{
		SetRange(GetRange() - NumRangeDecaying);
		NumRangeDecaying = 0;
	}
	if (NumPickupCostDecaying)
	{
		PickUpCost -= NumPickupCostDecaying;
		NumPickupCostDecaying = 0;
	}
}

/**
 * Undoes the effect of a resource that was sunk in this.
 *
 * @param FreedResource - The resource that was freed.
 */
void ATrash::ResourceFreed(UResource* FreedResource)
{
	switch (FreedResource->GetAllocationType())
	{
	case EResourceAllocationType::NotAllocated:
		UE_LOG(LogResource, Warning, TEXT("Cant free unallocated resource on  %s"), *GetName());
		return;
	case EResourceAllocationType::TrashCost:
		if (NumPickupCostDecaying)
		{
			NumPickupCostDecaying -= PickUpCostPerResource;
		}
		else
		{
			PickUpCost += PickUpCostPerResource;
		}
		break;
	case EResourceAllocationType::TrashDamage:
		if (NumDamageDecaying)
		{
			NumDamageDecaying -= DamagePerResource;
		}
		else
		{
			SetDamage(GetDamage() + DamagePerResource);
		}
		break;
	case EResourceAllocationType::TrashRange:
		if (NumRangeDecaying)
		{
			NumRangeDecaying -= RangePerResource;
		}
		else
		{
			SetRange(GetRange() + RangePerResource);
		}
		break;
	default:
		UE_LOG(LogResource, Error, TEXT("Tried to free a resource of non-trash type allocation from trash: %s"), *GetName());
		return;
	}
	AllocatedResources.RemoveSingle(FreedResource);
}

/* /\ Resource /\ *\
\* -------------- */

/* /\ ====== /\ *\
|  /\ ATrash /\  |
\* /\ ====== /\ */