// Fill out your copyright notice in the Description page of Project Settings.


#include "Trash.h"

#include "Syrup/Systems/SyrupGameMode.h"
#include "Effects/ApplyField.h"
//#include "Effects/DamagePlants.h"
#include "Components/InstancedStaticMeshComponent.h"

/* \/ ====== \/ *\
|  \/ ATrash \/  |
\* \/ ====== \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Creates the mesh and effect components.
 */
ATrash::ATrash()
{

}

/**
 * Binds effect triggers.
 */
void ATrash::BeginPlay()
{

}

/**
 * Initializes Damage, Range, and TimeUntilSpread; and sets the appropriate mesh.
 *
 * @param Transform - The new transform of the trash.
 */
void ATrash::OnConstruction(const FTransform& Transform)
{

}

/* /\ Initialization /\ *\
\* -------------------- */

/* ------------ *\
\* \/ Damage \/ */

/**
 * Updates the damage that this trash will deal.
 *
 * @param AmountAdded - The number of damage points to add to the current damage of this trash. May be a negative number
 */
void ATrash::UpdateDamage(int AmountAdded)
{
	Damage = FMath::Max(0, Damage + AmountAdded);
}

/* /\ Damage /\ *\
\* ------------ */

/* --------------- *\
\* \/ Spreading \/ */

/**
 * Spawns another trash of the same as this.
 */
void ATrash::Spread()
{

}

/* /\ Spreading /\ *\
\* --------------- */

/* ------------ *\
\* \/ Effect \/ */

/**
 * Activates the appropriate effects given the trigger.
 *
 * @param TriggerType - The type of trigger that was activated.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
 */
void ATrash::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& LocationsToTrigger)
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
		break;
	default:
		break;
	}

	TSet<FIntPoint> EffectedLocations = GetEffectLocations();
	TSet<FIntPoint> TriggeredLocations = LocationsToTrigger.IsEmpty() ? EffectedLocations : LocationsToTrigger.Intersect(EffectedLocations);

	TInlineComponentArray<UActorComponent*> Components = TInlineComponentArray<UActorComponent*>();
	GetComponents(UTileEffect::StaticClass(), Components);
	for (UActorComponent* EachComponent : Components)
	{
		Cast<UTileEffect>(EachComponent)->Affect(TriggerType, EffectedLocations);
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