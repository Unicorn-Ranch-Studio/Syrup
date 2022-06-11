// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupPlayerCharacter.h"

/* \/ ====================== \/ *\
|  \/ ASyrupPlayerCharacter  \/  |
\* \/ ====================== \/ */

/* -------------------- *\
\* \/ Initialization \/ */

// Sets default values
ASyrupPlayerCharacter::ASyrupPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASyrupPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASyrupPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASyrupPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASyrupPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASyrupPlayerCharacter::MoveRight);
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ---------------------- *\
\* \/ General Movement \/ */

/**
 * Adds a movement input in the forward direction
 * 
 * @param Scale - The scale of the axis
 */
void ASyrupPlayerCharacter::MoveForward(float Scale)
{
	MoveDirection(GetActorForwardVector(), Scale);
}

/**
 * Adds a movement input in the right direction
 * 
 * @param Scale - The scale of the axis
 */
void ASyrupPlayerCharacter::MoveRight(float Scale)
{
	MoveDirection(GetActorRightVector(), Scale);
}

/**
 * Handles a movement input in the given direction
 *
 * @param Direction - The direction of the input
 * @param Scale - The scale of the movement input
 */
void ASyrupPlayerCharacter::MoveDirection(FVector Direction, float Scale)
{
	AddMovementInput(Direction, Scale);
}

/* /\ General Movement /\ *\
\* ---------------------- */

/* /\ ===================== /\ *\
|  /\ ASyrupPlayerCharacter /\  |
\* /\ ===================== /\ */
