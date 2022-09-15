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
bool APlant::TakeDamage(int Amount)
{
	Health -= FMath::Max(0, Amount);
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
	return Data->GetShape();
}

/**
 * Binds effect triggers.
 */
void APlant::BeginPlay()
{
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

	Mesh->SetStaticMesh(Data->GetMesh());
	Health = Data->GetMaxHealth();
	TimeUntilGrown = Data->GetTimeUntilGrown() + 1;

	TriggersToAffectors.Empty();
	for (UTileEffect* EachEffect : Data->GetEffects())
	{
		ETileEffectTrigger TriggerType = EachEffect->Trigger;

		if (!TriggersToAffectors.Contains(TriggerType))
		{
			TriggersToAffectors.Add(TriggerType, NewObject<UTileAffecterComponent>(this));
			TriggersToAffectors.FindRef(TriggerType)->RegisterComponent();
		}

		TriggersToAffectors.FindRef(TriggerType)->Effects.Add(EachEffect);
	}

	Grow();
}

/**
 * Activates the appropriate effects given the trigger.
 *
 * @param TriggerType - The type of trigger that was activated.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect.
 */
void APlant::ReceiveEffectTrigger(ETileEffectTrigger TriggerType, TSet<FIntPoint> LocationsToTrigger)
{
	switch (TriggerType)
	{
	case ETileEffectTrigger::Persistent:
		break;
	case ETileEffectTrigger::PlantActive:
		break;
	case ETileEffectTrigger::TrashDamage:
		break;
	case ETileEffectTrigger::TrashActive:
		break;
	case ETileEffectTrigger::TrashSpread:
		break;
	case ETileEffectTrigger::PlantsGrow:
		Grow();
		break;
	default:
		break;
	}

	if (IsGrown())
	{
		TriggersToAffectors.FindRef(TriggerType)->ApplyEffect();
	}
}

/**
 * Gets the locations where the effects of this plant will apply.
 *
 * @return A set of all locations where the effects of this plant will apply.
 */
TSet<FIntPoint> APlant::GetEffectLocation() const
{
	return UGridLibrary::ScaleShapeUp(Data->GetShape(), Data->GetRange());
}

/**
 * Updates the plants so that it is 1 turn closer to fully grown, and causes the effects of being fully grown if needed.
 */
void APlant::Grow()
{
	if (!IsGrown())
	{
		TimeUntilGrown--;
		Mesh->SetRelativeScale3D(FVector(1 / (1 + TimeUntilGrown)));
		if (IsGrown())
		{
			TriggersToAffectors.FindRef(ETileEffectTrigger::Persistent)->ApplyEffect();
		}
	}
}
/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */