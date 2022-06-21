// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Syrup/GameManagement/InteractInterface.h"
#include "SyrupPlayerCharacter.generated.h"

class APlant;

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

private:

	/**
	 * Sets up the interact sphere component
	 */
	void SetupInteractCollision();

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

	/* ------------ *\
	\* \/ Energy \/ */

public:

	/**
	 * Gets the current energy of the player
	 * 
	 * @return The current energy
	 */
	UFUNCTION(BlueprintPure)
	int GetCurrentEnergy();

	/**
	 * Gets the maximum energy of the player
	 * 
	 * @return The maximum energy
	 */
	UFUNCTION(BlueprintPure)
	int GetMaximumEnergy();

protected:

	//The maximum/starting energy of the player
	UPROPERTY(EditDefaultsOnly)
	int MaximumEnergy;

private:

	//The current energy of the player
	int CurrentEnergy;

//temp public marker
//public:

	/**
	 * Attempts to use the given amount of energy. 
	 * 
	 * @param EnergyCost - The amount of energy used
	 * 
	 * @return Whether there was enough energy to use or not
	 */
	UFUNCTION()
	bool UseEnergy(int EnergyCost);

	/* /\ Energy /\ *\
	\* ------------ */

	/* -------------- *\
	\* \/ Planting \/ */

private:

//temp public marker
public:

	/**
	 * Plants a plant, using the energy the plant requires to plant
	 * 
	 * @param PlantClass - The class of the plant to be planted
	 * @param Location - The location to plant the plant
	 * 
	 * @return Whether the planting was sucessful or not
	 */
	UFUNCTION(BlueprintCallable)
	bool Plant(TSubclassOf<APlant> PlantClass, FVector Location);

	/* ----------------- *\
	\* \/ Interaction \/ */

protected:

	//Stores the radius of the character interaction collision
	UPROPERTY(EditDefaultsOnly)
	float InteractRadius;

private:

	/**
	 * Function that handles the interact sphere overlaps. Checks if the overlapped actor is interactable.
	 * 
	 * @param OverlappedComp - The component of this actor that was overalapped 
	 * @param OtherActor - The actor that overlapped this actor
	 * @param OtherComp - The specific component of the other actor that overlapped this actor
	 * @param OtherBodyIndex - The body index of the other actor
	 * @param bFromSweep - Whether this overlap was from a sweep or not
	 * @param SweepResult - The sweep result (if this was from a sweep)
	 */
	UFUNCTION()
	void OnInteractSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * Function that handles the interact sphere overlaps. Checks if the overlapped actor is interactable.
	 *
	 * @param OverlappedComp - The component of this actor that was un-overlapped
	 * @param OtherActor - The actor that un-overlapped this actor
	 * @param OtherComp - The specific component of the other actor that un-overlapped this actor
	 * @param OtherBodyIndex - The body index of the other actor
	 */
	UFUNCTION()
	void OnInteractSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**
	 * Handles when the interact button is pressed (as opposed to clicking on something with the mouse)
	 */
	UFUNCTION()
	void InteractPressed();

	/**
	 * The interaction sphere collision component
	 */
	UPROPERTY()
	USphereComponent* InteractCollision;

	/**
	 * The interactable actors that are currently within the interaction radius
	 */
	TArray<AActor*> InteractableActors;

	/* /\ Interaction /\ *\
	\* ----------------- */

};

/* /\ ===================== /\ *\
|  /\ ASyrupPlayerCharacter /\  |
\* /\ ===================== /\ */