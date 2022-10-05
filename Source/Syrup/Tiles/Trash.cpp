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
 * Creates the mesh and effect components.
 */
ATrash::ATrash()
{
	//Init Grass
	GoopComponent = CreateDefaultSubobject<UApplyField>(FName("Goop Zone"));
	GoopComponent->FieldType = EFieldType::Damage;

	//Init Prevent Trash Spawn
	DamageComponent = CreateDefaultSubobject<UDamagePlants>(FName("Damage Zone"));
	UpdateDamage();
}

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

	Range = GetRange();
	Shape.Add(FIntPoint::ZeroValue);
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
	DamageComponent->SetDamage(Damage);
}

/* /\ Damage /\ *\
\* ------------ */



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
 * Activates the appropriate effects given the trigger.
 *
 * @param TriggerType - The type of trigger that was activated.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
 */
void ATrash::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& LocationsToTrigger)
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