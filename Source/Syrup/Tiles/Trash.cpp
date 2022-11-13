// Fill out your copyright notice in the Description page of Project Settings.


#include "Trash.h"

#include "Syrup/Systems/SyrupGameMode.h"
#include "Effects/ApplyField.h"
#include "Effects/DamagePlants.h"
#include "Components/InstancedStaticMeshComponent.h"

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
	ReceiveEffectTrigger(ETileEffectTriggerType::OnActivated, TSet<FIntPoint>()); 
}

/**
 * Binds effect triggers.
 */
void ATrash::BeginPlay()
{
	Super::BeginPlay();

	ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::TrashSpawned, GetSubTileLocations());
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

	Range = GetRange();
	Shape.Add(FIntPoint::ZeroValue);
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
		ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::TrashPickedUp, GetSubTileLocations());
		ReceiveEffectTrigger(ETileEffectTriggerType::OnDeactivated, TSet<FIntPoint>());
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
void ATrash::SetRange(int NewRange)
{
	TSet<FIntPoint> OldEffectLocations = GetEffectLocations();
	Range = FMath::Max(0, NewRange);
	TSet<FIntPoint> NewEffectLocations = GetEffectLocations();
	ReceiveEffectTrigger(ETileEffectTriggerType::OnDeactivated, OldEffectLocations.Difference(NewEffectLocations));
	ReceiveEffectTrigger(ETileEffectTriggerType::OnActivated, NewEffectLocations.Difference(OldEffectLocations));
}

/**
 * Activates the appropriate effects given the trigger.
 *
 * @param TriggerType - The type of trigger that was activated.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
 */
void ATrash::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& LocationsToTrigger)
{
	TSet<FIntPoint> EffectedLocations = GetEffectLocations();
	TSet<FIntPoint> TriggeredLocations = LocationsToTrigger.IsEmpty() ? EffectedLocations : LocationsToTrigger.Intersect(EffectedLocations);

	if (!TriggeredLocations.IsEmpty())
	{
		TInlineComponentArray<UActorComponent*> Components = TInlineComponentArray<UActorComponent*>();
		GetComponents(UTileEffect::StaticClass(), Components);
		for (UActorComponent* EachComponent : Components)
		{
			Cast<UTileEffect>(EachComponent)->ActivateEffect(TriggerType, TriggeredLocations);
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

/* /\ ====== /\ *\
|  /\ ATrash /\  |
\* /\ ====== /\ */