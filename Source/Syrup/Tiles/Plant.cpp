// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"

#include "Syrup/Systems/SyrupGameMode.h"
#include "Effects/ApplyField.h"
#include "Effects/PreventTrashSpawn.h"
#include "Components/InstancedStaticMeshComponent.h"

DEFINE_LOG_CATEGORY(LogPlant);

/* \/ ====== \/ *\
|  \/ APlant \/  |
\* \/ ====== \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Creates the plants components.
 */
APlant::APlant()
{
	//Init Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Plant Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->AttachToComponent(SubtileMesh, FAttachmentTransformRules::KeepRelativeTransform);

	//Init Grass
	GrassComponent = CreateDefaultSubobject<UApplyField>(FName("Grass Zone"));
	GrassComponent->FieldType = EFieldType::Protection;

	//Init Prevent Trash Spawn
	PreventTrashComponent = CreateDefaultSubobject<UPreventTrashSpawn>(FName("Protection Zone"));

	//Get Plant Mat
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatRef(TEXT("/Game/Tiles/Plants/MI_Plant.MI_Plant"));
	TileMaterial = MatRef.Object;
	check(TileMaterial != nullptr);

	SubtileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

/**
 * Binds effect triggers and initializes size.
 */
void APlant::BeginPlay()
{
	Super::BeginPlay();

	TimeUntilGrown = GetTimeUntilGrown() + 1;
	Grow();

	ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::PlantSpawned, GetSubTileLocations());
	ASyrupGameMode::GetTileEffectTriggerDelegate(this).AddDynamic(this, &APlant::ReceiveEffectTrigger);
}

/**
 * Initializes Health, Range, and Shape; and sets the appropriate mesh.
 *
 * @param Transform - The new transform of the plant.
 */
void APlant::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	MeshComponent->SetStaticMesh(GetMesh());
	Health = GetMaxHealth();
	Range = GetRange();
	Shape.Add(FIntPoint::ZeroValue);
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
		ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::PlantKilled, GetSubTileLocations());
		ReceiveEffectTrigger(ETileEffectTriggerType::OnDeactivated, TSet<FIntPoint>());
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
void APlant::Grow_Implementation()
{
	if (!IsGrown())
	{
		TimeUntilGrown--;

		if (IsGrown())
		{
			ReceiveEffectTrigger(ETileEffectTriggerType::OnActivated, TSet<FIntPoint>());
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
	if(TriggerType == ETileEffectTriggerType::PlantsGrow)
	{
		Grow();
	}

	if (IsGrown() || TriggerType == ETileEffectTriggerType::PlantsGrow)
	{
		TSet<FIntPoint> EffectedLocations = GetEffectLocations();
		TSet<FIntPoint> TriggeredLocations = LocationsToTrigger.IsEmpty() ? EffectedLocations : LocationsToTrigger.Intersect(EffectedLocations);

		TInlineComponentArray<UActorComponent*> Components = TInlineComponentArray<UActorComponent*>();
		GetComponents(UTileEffect::StaticClass(), Components);
		for (UActorComponent* EachComponent : Components)
		{
			Cast<UTileEffect>(EachComponent)->ActivateEffect(TriggerType, EffectedLocations);
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