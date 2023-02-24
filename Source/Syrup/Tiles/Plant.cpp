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
APlant::APlant()
{
	FSinkAmountUpdateDelegate AmountSetter;
	AmountSetter.BindUFunction(this, FName("SetHealth"));
	FSinkLocationsDelegate LocationGetter;
	LocationGetter.BindUFunction(this, FName("GetSubTileLocations"));
	FSinkAmountDelegate AmountGetter;
	AmountGetter.BindUFunction(this, FName("GetHealth"));

	HealthResourceSink = UResourceSink::CreateDefaultResourceSinkComponent(this, AmountSetter, LocationGetter, AmountGetter);
	AmountSetter.BindUFunction(this, FName("SetRange"));
	AmountGetter.BindUFunction(this, FName("GetRange"));
	RangeResourceSink = UResourceSink::CreateDefaultResourceSinkComponent(this, AmountSetter, LocationGetter, AmountGetter);
	AmountSetter.BindUFunction(this, FName("SetProduction"));
	AmountGetter.BindUFunction(this, FName("GetProduction"));
	ProductionResourceSink = UResourceSink::CreateDefaultResourceSinkComponent(this, AmountSetter, LocationGetter, AmountGetter);
}


/**
 * Binds effect triggers and initializes size.
 */
void APlant::BeginPlay()
{
	Super::BeginPlay();

	SubtileMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	bIsFinishedPlanting = ASyrupGameMode::IsPlayerTurn(this);

	ASyrupGameMode::GetTileEffectTriggerDelegate(this).AddDynamic(this, &APlant::ReceiveEffectTrigger);
	ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::PlantSpawned, this, GetSubTileLocations());
}

/**
 * Handles undoing effects and deallocating resources,
 */
void APlant::Destroyed()
{
	Died_Implementation();
	Super::Destroyed();
}

/**
 * Initializes Health, Range, and Shape; and sets the appropriate mesh.
 *
 * @param Transform - The new transform of the plant.
 */
void APlant::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Health = 0;
	Range = 0;
	Production = 0;

	SubtileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

/* /\ Initialization /\ *\
\* -------------------- */



/* ------------ *\
\* \/ Health \/ */

/**
 * Causes this plant to take damage on the next trash damage cycle.
 *
 * @param Amount - The number of damage points to damage this plant by.
 * @param Cause - The tile that caused this damage.
 */
void APlant::NotifyIncomingDamage(int Amount, ATile* Cause)
{
	if (!bIsFinishedPlanting || !IsValid(Cause))
	{
		return;
	}

	int IncomingAmount = Amount;
	if(TilesToIncomingDamages.Contains(Cause))
	{
		IncomingAmount += TilesToIncomingDamages.FindRef(Cause);
	}
	if (IncomingAmount <= 0)
	{
		TilesToIncomingDamages.Remove(Cause);
	}
	TilesToIncomingDamages.Add(Cause, IncomingAmount);
	OnIncomingDamageRecived(Amount, Cause);
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
		Died();
	}
	else
	{
		if (NewHealth < Health)
		{
			Health = NewHealth;
			Damaged();
		}
		else
		{
			Health = NewHealth;
		}
	}
}

/**
 * Causes the effects of this plants death.
 */
void APlant::Died_Implementation()
{
	if (!bHasDied)
	{
		bHasDied = true;
		for (UResource* EachAllocatedResource : AllocatedResources)
		{
			EachAllocatedResource->Free();
		}
		for (UResource* EachAllocatedResource : ProducedResources)
		{
			EachAllocatedResource->Free();
		}

		SubtileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ReceiveEffectTrigger(ETileEffectTriggerType::OnDeactivated, nullptr, TSet<FIntPoint>());
	}
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
void APlant::ReceiveEffectTrigger_Implementation(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger)
{
	if (!bIsFinishedPlanting && TriggerType == ETileEffectTriggerType::PlayerTurn)
	{
		bIsFinishedPlanting = true;
	}

	if (bIsFinishedPlanting && TriggerType == ETileEffectTriggerType::TrashDamage)
	{
		int IncomingDamage = 0;
		for (TPair<ATile*, int> TileToIncomingDamage : TilesToIncomingDamages)
		{
			IncomingDamage += TileToIncomingDamage.Value;
		}
		DamageTaken += IncomingDamage;
		if (Health <= DamageTaken)
		{
			ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::PlantKilled, this, GetSubTileLocations());
			Died();
		}
		else if (IncomingDamage > 0)
		{
			Damaged();
		}
		TilesToIncomingDamages.Empty();
	}

	if (GetRange() >= 0 && Health > 0 && bIsFinishedPlanting)
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
	if (Range > 0)
	{
		return UGridLibrary::ScaleShapeUp(GetSubTileLocations(), Range);
	}
	
	return TSet<FIntPoint>();
}

/* /\ Effect /\ *\
\* ------------ */



/* -------------- *\
\* \/ Resource \/ */

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
		ProducedResources.Add(UResource::Create(ProductionType, this));
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

/* /\ Resource /\ *\
\* -------------- */

/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */