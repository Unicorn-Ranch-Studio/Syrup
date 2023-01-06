// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"

#include "Syrup/Systems/SyrupGameMode.h"
#include "Effects/TileEffect.h"
#include "Resources/Resource.h"
#include "Components/InstancedStaticMeshComponent.h"

DEFINE_LOG_CATEGORY(LogPlant);

/* \/ ====== \/ *\
|  \/ APlant \/  |
\* \/ ====== \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Binds effect triggers and initializes size.
 */
void APlant::BeginPlay()
{
	Super::BeginPlay();

	if (bStartGrown)
	{
		Range = 0;
		SetRange(GetMaxRange());

		Production = 0;
		SetProduction(GetMaxProduction());
	}
	else
	{
		Health = InitialHealth;

		Range = 0;
		SetRange(InitialRange);

		Production = 0;
		SetProduction(InitialProduction);
	}

	SubtileMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	bIsFinishedPlanting = true;
	Grow();
	bIsFinishedPlanting = ASyrupGameMode::IsPlayerTurn(this);

	ASyrupGameMode::GetTileEffectTriggerDelegate(this).AddDynamic(this, &APlant::ReceiveEffectTrigger);
	ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::PlantSpawned, this, GetSubTileLocations());
}

/**
 * Initializes Health, Range, and Shape; and sets the appropriate mesh.
 *
 * @param Transform - The new transform of the plant.
 */
void APlant::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SubtileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

/* /\ Initialization /\ *\
\* -------------------- */



/* ------------ *\
\* \/ Health \/ */

/**
 * Causes this plant to take damage.
 *
 * @param Amount - The number of damage points to damage this plant by.
 * @param Cause - The tile that caused this damage.
 *
 * @return Whether or not this plant was killed by the damage.
 */
bool APlant::ReceiveDamage(int Amount, ATile* Cause)
{
	if (!bIsFinishedPlanting)
	{
		return false;
	}

	DamageTaken += FMath::Max(0, Amount);
	bool bDead = Health <= DamageTaken;
	if (bDead && DamageTaken - Amount < Health)
	{
		Die();
	}
	OnDamageRecived(Amount, Cause, bDead);
	return bDead;
}


/**
 * Updates this plant to have the new amount of health.
 *
 * @param NewHealth - The new health of this plant.
 */
void APlant::SetHealth_Implementation(int NewHealth)
{
	if (DamageTaken > NewHealth && Health > DamageTaken)
	{
		Health = NewHealth;
		Die();
	}
	else
	{
		Health = NewHealth;
	}
}

/**
 * Causes the effects of this plants death.
 */
void APlant::Die()
{
	for (UResource* EachAllocatedResource : AllocatedResources)
	{
		EachAllocatedResource->Free();
	}
	for (UResource* EachAllocatedResource : ProducedResources)
	{
		EachAllocatedResource->Free();
	}

	SubtileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::PlantKilled, this, GetSubTileLocations());
	ReceiveEffectTrigger(ETileEffectTriggerType::OnDeactivated, nullptr, TSet<FIntPoint>());
}

/* /\ Health /\ *\
\* ------------ */



/* ------------ *\
\* \/ Growth \/ */

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
bool APlant::SowPlant(UObject* WorldContextObject, int& EnergyReserve, TSubclassOf<APlant> PlantClass, FTransform Transform)
{
	return SowPlant(WorldContextObject, EnergyReserve, PlantClass, UGridLibrary::WorldTransformToGridTransform(Transform));
}
bool APlant::SowPlant(UObject* WorldContextObject, int& EnergyReserve, TSubclassOf<APlant> PlantClass, FGridTransform Transform)
{
	APlant* DefaultPlant = PlantClass.GetDefaultObject();
	int NeededEnergy = DefaultPlant->GetPlantingCost();
	if (EnergyReserve >= NeededEnergy)
	{
		if (SowPlant(WorldContextObject, PlantClass, Transform))
		{
			EnergyReserve -= NeededEnergy;
			return true;
		}
	}
	return false;
}
bool APlant::SowPlant(UObject* WorldContextObject, TSubclassOf<APlant> PlantClass, FTransform Transform)
{
	return SowPlant(WorldContextObject, PlantClass, UGridLibrary::WorldTransformToGridTransform(Transform));
}
bool APlant::SowPlant(UObject* WorldContextObject, TSubclassOf<APlant> PlantClass, FGridTransform Transform)
{
	if (!IsValid(PlantClass) || PlantClass.Get()->HasAnyClassFlags(CLASS_Abstract))
	{
		UE_LOG(LogPlant, Error, TEXT("Tried to sow null or abstract plant."))
		return false;
	}
	
	TSet<ATile*> BlockingTiles;
	if (!UGridLibrary::OverlapShape(WorldContextObject, UGridLibrary::TransformShape(PlantClass.GetDefaultObject()->GetRelativeSubTileLocations(), Transform), BlockingTiles, TArray<AActor*>(), ECollisionChannel::ECC_GameTraceChannel3))
	{
		WorldContextObject->GetWorld()->SpawnActor<APlant>(PlantClass, UGridLibrary::GridTransformToWorldTransform(Transform));
		return true;
	}

	return false;
}

/**
 * Updates the plants so that it is 1 turn closer to fully grown, and causes the effects of being fully grown if needed.
 */
void APlant::Grow_Implementation()
{
	if (!bIsFinishedPlanting)
	{
		return;
	}

	if (NumHealthGrowing)
	{
		SetHealth(GetHealth() + NumHealthGrowing);
		NumHealthGrowing = 0;
	}
	if (NumRangeGrowing)
	{
		SetRange(GetRange() + NumRangeGrowing);
		NumRangeGrowing = 0;
	}
	if (NumProductionGrowing)
	{
		SetProduction(GetProduction() + NumProductionGrowing);
		NumProductionGrowing = 0;
	}
}

/* /\ Growth /\ *\
\* ------------ */



/* ------------ *\
\* \/ Effect \/ */

/**
 * Sets the range of this plant's effects.
 *
 * @param NewRange - The value to set the range to. Will be clamped >= 0.
 */
void APlant::SetRange(const int NewRange)
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
void APlant::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger)
{
	if(TriggerType == ETileEffectTriggerType::PlantsGrow)
	{
		Grow();
	} 
	else if (!bIsFinishedPlanting && TriggerType == ETileEffectTriggerType::PlayerTurn)
	{
		bIsFinishedPlanting = true;
	}

	if ((GetRange() >= 0 || TriggerType == ETileEffectTriggerType::PlantsGrow) && Health > 0)
	{
		TSet<FIntPoint> EffectedLocations = GetEffectLocations();
		TSet<FIntPoint> TriggeredLocations = LocationsToTrigger.IsEmpty() ? EffectedLocations : LocationsToTrigger.Intersect(EffectedLocations);

		TInlineComponentArray<UActorComponent*> Components = TInlineComponentArray<UActorComponent*>();
		GetComponents(UTileEffect::StaticClass(), Components);
		for (UActorComponent* EachComponent : Components)
		{
			Cast<UTileEffect>(EachComponent)->ActivateEffect(TriggerType, Triggerer, TriggeredLocations);
		}
	}
}

/**
 * Gets the locations where the effects of this plant will apply.
 *
 * @return A set of all locations where the effects of this plant will apply.
 */
TSet<FIntPoint> APlant::GetEffectLocations() const
{
	return UGridLibrary::ScaleShapeUp(GetSubTileLocations(), Range);
}

/* /\ Effect /\ *\
\* ------------ */



/* -------------- *\
\* \/ Resource \/ */

/**
 * Gets whether or not this can grow more health.
 *
 * @return Whether or not this can grow more health.
 */
bool APlant::CanGrowHealth() const
{
	return NumHealthGrowing < HealthGrowthPerTurn * HealthPerResource && Health + NumHealthGrowing < GetMaxHealth();
}

/**
 * Gets whether or not this can grow more range.
 *
 * @return Whether or not this can grow more range.
 */
bool APlant::CanGrowRange() const
{
	return NumRangeGrowing < RangeGrowthPerTurn * RangePerResource && Range + NumRangeGrowing < GetMaxRange();
}

/**
 * Gets whether or not this can grow more production.
 *
 * @return Whether or not this can grow more production.
 */
bool APlant::CanGrowProduction() const
{
	return NumProductionGrowing < ProductionGrowthPerTurn * ProductionPerResource && Production + NumProductionGrowing < GetMaxProduction();
}

/**
 * Causes this plant to grow more health, and allocates the given resource.
 *
 * @param Resource - The resource used to grow this health.
 *
 * @return Whether or not this was successful at growing more health.
 */
bool APlant::GrowHealth(UResource* Resource)
{
	if (!CanGrowHealth())
	{
		return false;
	}

	Resource->Allocate(this, EResourceAllocationType::PlantHealth);
	AllocatedResources.Add(Resource);
	NumHealthGrowing = FMath::Min(NumHealthGrowing + HealthPerResource, HealthGrowthPerTurn * HealthPerResource);
	return true;
}

/**
 * Causes this plant to grow more range, and allocates the given resource.
 *
 * @param Resource - The resource used to grow this range.
 *
 * @return Whether or not this was successful at growing more range.
 */
bool APlant::GrowRange(UResource* Resource)
{
	if (!CanGrowRange())
	{
		return false;
	}

	Resource->Allocate(this, EResourceAllocationType::PlantRange);
	AllocatedResources.Add(Resource);
	NumRangeGrowing = FMath::Min(NumRangeGrowing + RangePerResource, RangeGrowthPerTurn * RangePerResource);
	return true;
}

/**
 * Causes this plant to grow more production, and allocates the given resource.
 *
 * @param Resource - The resource used to grow this production.
 *
 * @return Whether or not this was successful at growing more production.
 */
bool APlant::GrowProduction(UResource* Resource)
{
	if (!CanGrowProduction())
	{
		return false;
	}

	Resource->Allocate(this, EResourceAllocationType::PlantProduction);
	AllocatedResources.Add(Resource);
	NumProductionGrowing = FMath::Min(NumProductionGrowing + ProductionPerResource, ProductionGrowthPerTurn * ProductionPerResource);
	return true;
}

/**
 * Updates this plant to have the new amount of production.
 *
 * @param NewHealth - The new production of this plant.
 */
void APlant::SetProduction_Implementation(int NewProduction)
{
	NewProduction = FMath::Max(0, NewProduction);
	while (ProducedResources.Num() < NewProduction)
	{
		ProducedResources.Add(UResource::Create(this));
		Production++;
	}

	while (ProducedResources.Num() > NewProduction)
	{
		UResource* ResourceToRemove = nullptr;
		for (UResource* EachProducedResource : ProducedResources)
		{
			if (!EachProducedResource->IsAllocated())
			{
				ResourceToRemove = EachProducedResource;
				break;
			}
		}

		if (!IsValid(ResourceToRemove))
		{
			ProducedResources[ProducedResources.Num() - 1]->Free();
			ProducedResources.SetNum(ProducedResources.Num() - 1);
			Production--;
			return;
		}
		ResourceToRemove->Free();
		ProducedResources.RemoveSingle(ResourceToRemove);
		Production--;
	}
}
/**
 * Undoes the effect of a resource that was sunk in this.
 *
 * @param FreedResource - The resource that was freed.
 */
void APlant::ResourceFreed(UResource* FreedResource)
{
	switch (FreedResource->GetAllocationType())
	{
	case EResourceAllocationType::NotAllocated:
		UE_LOG(LogResource, Warning, TEXT("Cant free unallocated resource on  %s"), *GetName());
		return;
	case EResourceAllocationType::PlantHealth:
		if (NumHealthGrowing)
		{
			NumHealthGrowing -= HealthPerResource;
		}
		else
		{
			SetHealth(GetHealth() - HealthPerResource);
		}
		break;
	case EResourceAllocationType::PlantRange:
		if (NumRangeGrowing)
		{
			NumRangeGrowing -= RangePerResource;
		}
		else
		{
			SetRange(GetRange() - RangePerResource);
		}
		break;
	case EResourceAllocationType::PlantProduction:
		if (NumProductionGrowing)
		{
			NumProductionGrowing = ProductionPerResource;
		}
		else
		{
			SetProduction(GetProduction() - ProductionPerResource);
		}
		break;
	default:
		UE_LOG(LogResource, Error, TEXT("Tried to free a resource of non-plant type allocation from plant: %s"), *GetName());
		return;
	}

	if (Health > DamageTaken)
	{
		AllocatedResources.RemoveSingle(FreedResource);
	}
}

/* /\ Resource /\ *\
\* -------------- */

/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */