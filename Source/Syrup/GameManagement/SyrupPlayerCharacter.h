// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SyrupPlayerCharacter.generated.h"

/* \/ ====================== \/ *\
|  \/ ASyrupPlayerCharacter  \/  |
\* \/ ====================== \/ */

/**
 * A class that performs the actions of the player
 */
UCLASS()
class SYRUP_API ASyrupPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

public:
	// Sets default values for this character's properties
	ASyrupPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ---------------------- *\
	\* \/ General Movement \/ */

private:

	/**
	 * Adds a movement input in the forward direction
	 * 
	 * @param Scale - The scale of the axis
	 */
	UFUNCTION()
	void MoveForward(float Scale);

	/**
	 * Adds a movement input in the right direction
	 * 
	 * @param Scale - The scale of the axis
	 */
	UFUNCTION()
	void MoveRight(float Scale);

	/**
	 * Handles a movement input in the given direction
	 * 
	 * @param Direction - The direction of the input
	 * @param Scale - The scale of the movement input
	 */
	UFUNCTION()
	void MoveDirection(FVector Direction, float Scale);

	/* /\ General Movement /\ *\
	\* ---------------------- */

};

/* /\ ===================== /\ *\
|  /\ ASyrupPlayerCharacter /\  |
\* /\ ===================== /\ */