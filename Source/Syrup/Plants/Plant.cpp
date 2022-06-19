// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"

/* \/ ======= \/ *\
|  \/ APlant  \/  |
\* \/ ======= \/ */

/* -------------------- *\
\* \/ Initialization \/ */

// Sets default values
APlant::APlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlant::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/* /\ Initialization /\ *\
\* -------------------- */

/* ------------ *\
\* \/ Health \/ */

/**
 * Kills this plant
 */
void APlant::Die()
{
	//idk what happens if plant dies
}

/**
 * Override of take damage to handle the damage taken. Negative damage can be applied to heal plants
 *
 * @param DamageAmount - The amount of damage taken
 * @param DamageEvent - Struct containing more information about the damage taken
 * @param EventInstigator - Who caused the damage
 * @param DamageCauser - What caused the damage
 *
 * @return The actual amount of damage taken
 */
float APlant::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Cast<UHealing>(DamageEvent.DamageTypeClass))
	{
		DamageAmount = DamageAmount * -1;
	}

	Health -= FMath::RoundFromZero(DamageAmount);

	if (Health <= 0)
	{
		Die();
		return DamageAmount + Health;
	}

	if (Health > MaxHealth)
	{
		Health = MaxHealth;
		return -1 * ((DamageAmount * -1) - Health);
	}

	return DamageAmount;
}

/* /\ Health /\ *\
\* ------------ */

/* ------------ *\
\* \/ Energy \/ */

/**
 * Gets the energy cost of planting this plant
 *
 * @return The energy cost of planting this plant
 */
int APlant::GetEnergyCostToPlant()
{
	return EnergyCostToPlant;
}

/* /\ Energy /\ *\
\* ------------ */

/* -------------- *\
\* \/ Maturity \/ */

/**
 * Changes the plant into its mature state
 */
void APlant::Mature()
{
	bMature = true;
}

/* /\ Maturity /\ *\
\* -------------- */

/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */