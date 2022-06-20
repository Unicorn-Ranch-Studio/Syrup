// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

/* -------------------- *\
\* \/ Initialization \/ */

// Sets default values
ANPC::ANPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/* /\ Initialization /\ *\
\* -------------------- */

/* ----------------- *\
\* \/ Interaction \/ */

/**
 * Interact override to handle interaction with NPCs
 */
void ANPC::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("i was interacted with???"))
}

/* /\ Interaction /\ *\
\* ----------------- */