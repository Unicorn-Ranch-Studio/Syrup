// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"

#include "Syrup/SyrupGameMode.h"
#include "Effects/TileEffect.h"
#include "Components/InstancedStaticMeshComponent.h"

DEFINE_LOG_CATEGORY(LogPlant);

/* \/ ====== \/ *\
|  \/ APlant \/  |
\* \/ ====== \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Creates the mesh component.
 */
APlant::APlant()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Plant Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->AttachToComponent(SubtileMesh, FAttachmentTransformRules::KeepRelativeTransform);

	//Get Plant Mat
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MeshRef(TEXT("/Game/Tiles/Plants/MI_Plant.MI_Plant"));
	TileMaterial = MeshRef.Object;
	check(TileMaterial != nullptr);

	SubtileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

/**
 * Binds effect triggers.
 */
void APlant::BeginPlay()
{
	Super::BeginPlay();

	ASyrupGameMode::GetTileEffectTriggerDelegate(this).AddDynamic(this, &APlant::ReceiveEffectTrigger);
}

/**
 * Initializes Health, Range, and TimeUntilGrown; and sets the appropriate mesh.
 *
 * @param Transform - The new transform of the plant.
 */
void APlant::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	MeshComponent->SetStaticMesh(GetMesh());
	Health = GetMaxHealth();
	TimeUntilGrown = GetTimeUntilGrown() + 1;
	Range = GetRange();
	Shape.Add(FIntPoint::ZeroValue);

	Grow();
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ----------- *\
\* \/ Shape \/ */

/*
 * The relative locations of all of the sub-tiles of this plant.
 *
 * @return The relative locations of all of the sub-tiles of this plant.
 */
TSet<FIntPoint> APlant::GetRelativeSubTileLocations() const
{
	return GetShape();
}

/* /\ Shape /\ *\
\* ----------- */

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
	Health -= FMath::Max(0, Amount);
	if (Health <= 0)
	{
		Destroy();
	}
	return Health <= 0;
}

/* /\ Health /\ *\
\* ------------ */

/* ------------ *\
\* \/ Growth \/ */

/**
 * Updates the plants so that it is 1 turn closer to fully grown, and causes the effects of being fully grown if needed.
 */
void APlant::Grow()
{
	if (!IsGrown())
	{
		TimeUntilGrown--;
		MeshComponent->SetRelativeScale3D(FVector(1.f / (1 + TimeUntilGrown)));
		if (IsGrown())
		{
			ReceiveEffectTrigger(ETileEffectTriggerType::Persistent, TSet<FIntPoint>());
		}
	}
}

/* /\ Growth /\ *\
\* ------------ */

/* ------------ *\
\* \/ Effect \/ */

/**
 * Activates the appropriate effects given the trigger.
 *
 * @param TriggerType - The type of trigger that was activated.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
 */
void APlant::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& LocationsToTrigger)
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

	if (IsGrown() || TriggerType == ETileEffectTriggerType::PlantsGrow)
	{
		TSet<FIntPoint> EffectedLocations = LocationsToTrigger;
		if (EffectedLocations.IsEmpty())
		{
			EffectedLocations = GetEffectLocations();
		}

		TInlineComponentArray<UActorComponent*> Components = TInlineComponentArray<UActorComponent*>();
		GetComponents(UTileEffect::StaticClass(), Components);
		for (UActorComponent* EachComponent : Components)
		{
			Cast<UTileEffect>(EachComponent)->Affect(TriggerType, EffectedLocations);
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

/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */