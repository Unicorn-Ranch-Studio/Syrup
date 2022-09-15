// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "SyrupPlayerCharacter.generated.h"

/* \/ ===================== \/ *\
|  \/ ASyrupPlayerCharacter \/  |
\* \/ ===================== \/ */
UCLASS()
class SYRUP_API ASyrupPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/* ---------------- *\
	\* \/ Components \/ */

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* CameraRoot;
	
	/* /\ Components /\ *\
	\* ---------------- */


public:
	// Sets default AxisValues for this character's properties
	ASyrupPlayerCharacter();

	/* --------------- *\
	\* \/ Variables \/ */
private:
	//Stores most recent X and Y input values from MoveForward() and MoveRight()
	float recentInputX = 0.f, recentInputY = 0.f;

	/* /\ Variables /\ *\
	\* --------------- */


	/* --------------- *\
	\* \/ Functions \/ */
public:	
	// Called to bind functionality to input
	/**
	 * Sets up movement axis and inputs
	 *
	 * @param UInputComponent*
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/**
	 * Moves ASyrupPlayerCharacter by the movement speed multiplied the input axis values.
	 *
	 * @param AxisValue - Takes input axis value of "MoveForward" and "MoveRight".
	 */
	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	/* /\ Functions /\ *\
	\* --------------- */
};
/* /\ ===================== /\ *\
|  /\ ASyrupPlayerCharacter /\  |
\* /\ ===================== /\ */