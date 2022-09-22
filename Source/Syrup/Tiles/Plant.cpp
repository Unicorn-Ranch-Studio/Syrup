// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"

#include "Effects/TileEffectTrigger.h"
#include "Effects/TileEffect.h"
#include "Effects/TileAffecterComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

/* \/ ====== \/ *\
|  \/ APlant \/  |
\* \/ ====== \/ */
/**
 * Creates the mesh component.
 */
APlant::APlant()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Plant Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->AttachToComponent(SubtileMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

/**
 * Causes this plant to take damage.
 *
 * @param Amount - The number of damage points to damage this plant by.
 *
 * @return Whether or not this plant was killed by the damage.
 */
bool APlant::ReceiveDamage(int Amount)
{
	Health -= FMath::Max(0, Amount);
	if (Health <= 0)
	{
		Destroy();
	}
	return Health <= 0;
}

/**
 * Gets whether or not this plant is fully grown.
 *
 * @return Whether or not this plant is fully grown.
 */
bool APlant::IsGrown() const
{
	return TimeUntilGrown <= 0;
}

/*
 * The relative locations of all of the sub-tiles of this plant.
 *
 * @return The relative locations of all of the sub-tiles of this plant.
 */
TSet<FIntPoint> APlant::GetRelativeSubTileLocations() const
{
	if (IsValid(Data))
	{
		return Data->GetShape();
	}
	return Super::GetRelativeSubTileLocations();
}

/**
 * Binds effect triggers.
 */
void APlant::BeginPlay()
{
	Super::BeginPlay();
	//TODO: Implement bindings to ReciveEffectTriggers once turn system is completed.
}

/**
 * Initializes Health, Range, and TimeUntilGrown; and sets the appropriate mesh.
 *
 * @param Transform - The new transform of the plant.
 */
void APlant::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (IsValid(Data))
	{
		Mesh->SetStaticMesh(Data->GetMesh());
		Health = Data->GetMaxHealth();
		TimeUntilGrown = Data->GetTimeUntilGrown() + 1;
		Range = Data->GetRange();

		TriggersToAffectors.Empty();
		for (UTileEffect* EachEffect : Data->GetEffects())
		{
			ETileEffectTriggerType TriggerType = EachEffect->Trigger;

			if (!TriggersToAffectors.Contains(TriggerType))
			{
				TriggersToAffectors.Add(TriggerType, NewObject<UTileAffecterComponent>(this));
				TriggersToAffectors.FindRef(TriggerType)->RegisterComponent();
			}

			TriggersToAffectors.FindRef(TriggerType)->Effects.Add(EachEffect);
		}

		Grow();
	}
}

/**
 * Activates the appropriate effects given the trigger.
 *
 * @param TriggerType - The type of trigger that was activated.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
 */
void APlant::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint> LocationsToTrigger)
{
	switch (TriggerType)
	{
	case ETileEffectTriggerType::Persistent:
		break;
	case ETileEffectTriggerType::PlantActive:
		break;
	case ETileEffectTriggerType::TrashDamage:
		break;
	case ETileEffectTriggerType::TrashActive:
		break;
	case ETileEffectTriggerType::TrashSpread:
		break;
	case ETileEffectTriggerType::PlantsGrow:
		Grow();
		break;
	default:
		break;
	}

	if (IsGrown() && TriggersToAffectors.Contains(TriggerType))
	{
		const TSet<FIntPoint> EffectLocations = GetEffectLocations();
		if (LocationsToTrigger.IsEmpty())
		{
			TriggersToAffectors.FindRef(TriggerType)->ApplyEffect(EffectLocations);
		}
		else
		{
			TriggersToAffectors.FindRef(TriggerType)->ApplyEffect(LocationsToTrigger.Union(EffectLocations));
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

/**
 * Updates the plants so that it is 1 turn closer to fully grown, and causes the effects of being fully grown if needed.
 */
void APlant::Grow()
{
	if (!IsGrown())
	{
		TimeUntilGrown--;
		Mesh->SetRelativeScale3D(FVector(1.f / (1 + TimeUntilGrown)));
		if (IsGrown() && TriggersToAffectors.Contains(ETileEffectTriggerType::Persistent))
		{
			TriggersToAffectors.FindRef(ETileEffectTriggerType::Persistent)->ApplyEffect(GetEffectLocations());
		}
	}
}
/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */