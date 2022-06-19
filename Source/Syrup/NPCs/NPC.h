// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC.generated.h"

/* \/ ==== \/ *\
|  \/ ANPC  \/  |
\* \/ ==== \/ */

/**
 * The base class for the NPCs
 */
UCLASS()
class SYRUP_API ANPC : public AActor
{
	GENERATED_BODY()
	
	/* -------------------- *\
	\* \/ Initialization \/ */

public:	
	// Sets default values for this actor's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* -------------- *\
	\* \/ Dialogue \/ */

public:

	//Temporary variable for interaction testing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Dialogue;

	/* /\ Dialogue /\ *\
	\* -------------- */

};

/* /\ ==== /\ *\
|  /\ ANPC /\  |
\* /\ ==== /\ */