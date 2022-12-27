// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldType.generated.h"


/* \/ ========== \/ *\
|  \/ EFieldType \/  |
\* \/ ========== \/ */
UENUM(BlueprintType)
enum class EFieldType : uint8
{
	Protection	UMETA(DisplayName = "Protection Range"),
	Damage		UMETA(DisplayName = "Damage Range"),
	Hole		UMETA(DisplayName = "Hole in Ground")
};
/* /\ ========== /\ *\
|  /\ EFieldType /\  |
\* /\ ========== /\ */