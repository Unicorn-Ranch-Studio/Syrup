// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SyrupPlayerCharacter.generated.h"

class UCameraComponent;

/* \/ ===================== \/ *\
|  \/ ASyrupPlayerCharacter \/  |
\* \/ ===================== \/ */
/**
 * The Player Character that can use energy, 
 * pick up trash, move arround, plant, and end the players turn.
 */
UCLASS()
class SYRUP_API ASyrupPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/* ---------------- *\
	\* \/ Components \/ */

private:
	//Camera of the player character.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;

	//Scene component of the player character.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* CameraRoot;
	
	/* /\ Components /\ *\
	\* ---------------- */


public:
	/**
	 * Sets up camera and movement components.
	 */
	ASyrupPlayerCharacter();

	/* ----------- *\
	\* \/ Input \/ */
public:	
	/**
	 * Sets up movement axis and inputs.
	 *
	 * @param PlayerInputComponent - The input component to set up.
	 */
	UFUNCTION()
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/**
	 * Moves ASyrupPlayerCharacter by the movement speed multiplied the input axis values.
	 *
	 * @param AxisValue - A value between 1 and -1 that determines what direction and percent of maximum speed this character will move in the next frame.
	 */
	UFUNCTION()
	void MoveForward(float AxisValue);
	
	/**
	* Moves ASyrupPlayerCharacter by the movement speed multiplied the input axis values.
	*
	* @param AxisValue - A value between 1 and -1 that determines what direction and percent of maximum speed this character will move in the next frame.
	*/
	UFUNCTION()
	void MoveRight(float AxisValue);

	/* /\ Input /\ *\
	\* ----------- */
};
/* /\ ===================== /\ *\
|  /\ ASyrupPlayerCharacter /\  |
\* /\ ===================== /\ */