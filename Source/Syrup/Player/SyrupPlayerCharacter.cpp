// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupPlayerCharacter.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Syrup/Systems/SyrupGameMode.h"



/* \/ ===================== \/ *\
|  \/ ASyrupPlayerCharacter \/  |
\* \/ ===================== \/ */

/**
 * Sets up camera and movement components.
 */
ASyrupPlayerCharacter::ASyrupPlayerCharacter()
{
 	//Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Define Resources
	Energy = 10;
	PlantSelected = 0;
	PlantTypes = { NULL };

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

	//Capsule Component
	
}

/**
 * Sets up movement axis and inputs.
 *
 * @param PlayerInputComponent - The input component to set up.
 */
void ASyrupPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASyrupPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASyrupPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction(TEXT("Plant"), IE_Pressed, this, &ASyrupPlayerCharacter::Plant);
	PlayerInputComponent->BindAction(TEXT("EndTurn"), IE_Pressed, this, &ASyrupPlayerCharacter::EndTurn);
}

/**
 *  Adds movement in the X Directions
 * 
 * @param Takes axis values from "MoveForward"
 */
void ASyrupPlayerCharacter::MoveForward(float AxisValue)
{
	GetCharacterMovement()->AddInputVector(FVector(AxisValue, 0.f, 0.f));
}

/**
* Moves ASyrupPlayerCharacter by the movement speed multiplied the input axis values.
*
* @param AxisValue - A value between 1 and -1 that determines what direction and percent of maximum speed this character will move in the next frame.
*/
void ASyrupPlayerCharacter::MoveRight(float AxisValue)
{
	GetCharacterMovement()->AddInputVector(FVector(0.f, AxisValue, 0.f));
}


void ASyrupPlayerCharacter::Plant()
{
	APlant::SowPlant(this, Energy, PlantTypes[PlantSelected], (ASyrupPlayerCharacter::GetTransform()));

	UE_LOG(LogTemp, Warning, TEXT("PLANT"));
}

void ASyrupPlayerCharacter::EndTurn()
{
	Energy = 10;
	ASyrupGameMode::EndPlayerTurn(this);

	UE_LOG(LogTemp, Warning, TEXT("EndTurn"));
}
/* /\ ===================== /\ *\
|  /\ ASyrupPlayerCharacter /\  |
\* /\ ===================== /\ */