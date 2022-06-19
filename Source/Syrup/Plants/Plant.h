// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Syrup/GameManagement/SyrupDamageTypes.h"
#include "Plant.generated.h"

/* \/ ======= \/ *\
|  \/ APlant  \/  |
\* \/ ======= \/ */

/**
 * The base class for the plants
 */
UCLASS()
class SYRUP_API APlant : public AActor
{
	GENERATED_BODY()
	

	/* -------------------- *\
	\* \/ Initialization \/ */

public:	
	// Sets default values for this actor's properties
	APlant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ------------ *\
	\* \/ Health \/ */

protected:

	//Stores the maximum/starting health of the plant
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Health"))
	int MaxHealth;

private:

	/**
	 * Kills this plant
	 */
	UFUNCTION()
	void Die();

	/**
	 * Override of take damage to handle the damage taken
	 *
	 * @param DamageAmount - The amount of damage taken
	 * @param DamageEvent - Struct containing more information about the damage taken
	 * @param EventInstigator - Who caused the damage
	 * @param DamageCauser - What caused the damage
	 *
	 * @return Not relavent
	 */
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//Stores the current health of the plant
	int Health;

	/* /\ Health /\ *\
	\* ------------ */

	/* ------------ *\
	\* \/ Energy \/ */

public:

	/**
	 * Gets the energy cost of planting this plant
	 * 
	 * @return The energy cost of planting this plant
	 */
	UFUNCTION(BlueprintPure)
	int GetEnergyCostToPlant();

	//The energy cost of planting this plant
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Energy"))
	int EnergyCostToPlant;

	/* /\ Energy /\ *\
	\* ------------ */

	/* -------------- *\
	\* \/ Maturity \/ */

protected:

	//The turns it takes for this plant to fully mature from when it was planted
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Maturity"))
	int NumberOfTurnsToMature;

private:

	/**
	 * Changes the plant into its mature state
	 */
	void Mature();

	//The number of turns left until this plant is fully mature
	int NumberOfTurnsUntilMaturity;

	//Whether this plant is mature or not
	bool bMature;

	/* /\ Maturity /\ *\
	\* -------------- */

	/* ----------------------- *\
	\* \/ Protection Radius \/ */

private:

	//The radius around the plant that no trash can spawn. Only active when the plant is mature.
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Protection Radius"))
	float ProtectionRadius;

	/* /\ Protection Radius /\ *\
	\* ----------------------- */
};

/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */
