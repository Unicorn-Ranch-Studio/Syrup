// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Syrup/Plants/Plant.h"

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

	SetupInteractCollision();

}

// Called when the game starts or when spawned
void ASyrupPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentEnergy = MaximumEnergy;
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
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ASyrupPlayerCharacter::InteractPressed);
}

/**
 * Sets up the interact sphere component
 */
void ASyrupPlayerCharacter::SetupInteractCollision()
{
	InteractCollision = CreateDefaultSubobject<USphereComponent>("InteractCollision");
	InteractCollision->SetupAttachment(RootComponent);

	InteractCollision->SetSphereRadius(InteractRadius);

	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &ASyrupPlayerCharacter::OnInteractSphereOverlap);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &ASyrupPlayerCharacter::OnInteractSphereEndOverlap);
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

/* ------------ *\
\* \/ Energy \/ */

/**
 * Gets the current energy of the player
 *
 * @return The current energy
 */
int ASyrupPlayerCharacter::GetCurrentEnergy()
{
	return CurrentEnergy;
}

/**
 * Gets the maximum energy of the player
 * 
 * @return The maximum energy
 */
int ASyrupPlayerCharacter::GetMaximumEnergy()
{
	return MaximumEnergy;
}

/**
 * Attempts to use the given amount of energy.
 *
 * @param EnergyCost - The amount of energy used
 *
 * @return Whether there was enough energy to use or not
 */
bool ASyrupPlayerCharacter::UseEnergy(int EnergyCost)
{
	if (EnergyCost > CurrentEnergy)
	{
		return false;
	}

	CurrentEnergy -= EnergyCost;
	return true;
}

/**
 * Plants a plant, using the energy the plant requires to plant
 *
 * @param PlantClass - The class of the plant to be planted
 * @param Location - The location to plant the plant
 *
 * @return Whether the planting was sucessful or not
 */
bool ASyrupPlayerCharacter::Plant(TSubclassOf<APlant> PlantClass, FVector Location)
{
	int EnergyCostToPlant = Cast<APlant>(PlantClass->ClassDefaultObject)->GetEnergyCostToPlant();

	if (UseEnergy(EnergyCostToPlant))
	{
		GetWorld()->SpawnActor<APlant>(PlantClass, Location, FRotator(), FActorSpawnParameters());
		return true;
	}

	return false;
}

/* /\ Energy /\ *\
\* ------------ */

/* ----------------- *\
\* \/ Interaction \/ */

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
void ASyrupPlayerCharacter::OnInteractSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractInterface* InteractableActor = Cast<IInteractInterface>(OtherActor);
	if (InteractableActor)
	{
		InteractableActor->BecomeInteractable();
		InteractableActors.Emplace(OtherActor);
	}
}

/**
 * Function that handles the interact sphere overlaps. Checks if the overlapped actor is interactable.
 *
 * @param OverlappedComp - The component of this actor that was un-overlapped
 * @param OtherActor - The actor that un-overlapped this actor
 * @param OtherComp - The specific component of the other actor that un-overlapped this actor
 * @param OtherBodyIndex - The body index of the other actor
 */
void ASyrupPlayerCharacter::OnInteractSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractInterface* InteractableActor = Cast<IInteractInterface>(OtherActor);
	if (InteractableActor)
	{
		InteractableActor->BecomeNotInteractable();
		InteractableActors.Remove(OtherActor);
	}
}

/**
 * Handles when the interact button is pressed (as opposed to clicking on something with the mouse)
 */
void ASyrupPlayerCharacter::InteractPressed()
{
	if (!InteractableActors.IsEmpty())
	{
		if (InteractableActors.Num() == 1)
		{
			Cast<IInteractInterface>(InteractableActors[0])->Interact();
		}
		else 
		{
			//Distance to the nearest actor (not useful in this case, but required for the function call)
			float Distance;
			AActor* NearestInteractableActor = UGameplayStatics::FindNearestActor(GetActorLocation(), InteractableActors, Distance);

			Cast<IInteractInterface>(NearestInteractableActor)->Interact();
		}
	}

}

/* /\ Interaction /\ *\
\* ----------------- */

/* /\ ===================== /\ *\
|  /\ ASyrupPlayerCharacter /\  |
\* /\ ===================== /\ */
