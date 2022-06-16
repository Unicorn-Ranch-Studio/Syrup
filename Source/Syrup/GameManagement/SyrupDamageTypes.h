// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "SyrupDamageTypes.generated.h"

/* \/ ========== \/ *\
|  \/ UDamaging  \/  |
\* \/ ========== \/ */

/**
 * The damaging damage type
 */
UCLASS()
class SYRUP_API UDamaging : public UDamageType
{
	GENERATED_BODY()
};

/* /\ ========= /\ *\
|  /\ UDamaging /\  |
\* /\ ========= /\ */

/* \/ ======== \/ *\
|  \/ UHealing \/  |
\* \/ ======== \/ */

/**
 * The healing damage type
 */
UCLASS()
class SYRUP_API UHealing : public UDamageType
{
	GENERATED_BODY()
};

/* /\ ======== /\ *\
|  /\ UHealing /\  |
\* /\ ======== /\ */