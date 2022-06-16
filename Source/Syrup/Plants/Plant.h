// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Syrup/GameManagement/SyrupDamageTypes.h"
#include "Plant.generated.h"

/* \/ ======= \/ *\
|  \/ APlant  \/  |
\* \/ ======= \/ */

UCLASS()
class SYRUP_API APlant : public AActor
{
	GENERATED_BODY()
	

	/* -------------------- *\
	\* \/ Initialization \/ */

public:	
	// Sets default values for this actor's properties
	APlant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

};

/* /\ ====== /\ *\
|  /\ APlant /\  |
\* /\ ====== /\ */
