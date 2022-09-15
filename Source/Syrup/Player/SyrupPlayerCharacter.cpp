// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupPlayerCharacter.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

/* \/ ===================== \/ *\
|  \/ ASyrupPlayerCharacter \/  |
\* \/ ===================== \/ */

/**
 * Creates Camera (with defaults) and its root so it doesn't rotate when changing direction
 */
ASyrupPlayerCharacter::ASyrupPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Camera Root
	CameraRoot = CreateDefaultSubobject<USceneComponent>(FName("Camera Root"));
	CameraRoot->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraRoot->SetAbsolute(false, true);


	//Create Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera Component"));
	Camera->AttachToComponent(CameraRoot, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetRelativeLocation(FVector(-200.0f, 0.0f, 550.0f));
	Camera->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	
	//Auto Posess
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Rotate on Acceleration
	bUseControllerRotationPitch, bUseControllerRotationRoll, bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

/**
 *  Called to bind functionality to input
 */
void ASyrupPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASyrupPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASyrupPlayerCharacter::MoveRight);

}

/**
 *  Adds movement in the X Directions
 * 
 * @param Takes axis values from "MoveForward"
 */
void ASyrupPlayerCharacter::MoveForward(float AxisValue)
{
	GetCharacterMovement()->AddInputVector(FVector(AxisValue, 0.f, 0.f));
	recentInputX = AxisValue;
}

/**
 *  Adds movement in the Y Directions
 *
 * @param Takes axis values from "MoveRight"
 */
void ASyrupPlayerCharacter::MoveRight(float AxisValue)
{
	GetCharacterMovement()->AddInputVector(FVector(0.f, AxisValue, 0.f));
	recentInputY = AxisValue;
}

/* /\ ===================== /\ *\
|  /\ ASyrupPlayerCharacter /\  |
\* /\ ===================== /\ */