// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SyrupFunctionLibrary.generated.h"

class ASyrupPlayerCharacter;

/* \/ ====================== \/ *\
|  \/ USyrupFunctionLibrary  \/  |
\* \/ ====================== \/ */

/**
 * 
 */
UCLASS()
class SYRUP_API USyrupFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Gets the syrup player character
	 * 
	 * @return The syrup player character
	 */
	UFUNCTION(BlueprintPure)
	ASyrupPlayerCharacter* GetSyrupPlayerCharacter();

};

/* /\ ===================== /\ *\
|  /\ USyrupFunctionLibrary /\  |
\* /\ ===================== /\ */