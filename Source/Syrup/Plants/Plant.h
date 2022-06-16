// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Syrup/GameManagement/SyrupDamageTypes.h"
#include "Plant.generated.h"

/* \/ ======= \/ *\
|  \/ APlant  \/  |
\* \/ ======= \/ */

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

	//The energy cost of planting this plant
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Energy"))
	int EnergyCostToPlant;

	/* /\ Energy /\ *\
	\* ------------ */
};

/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */
