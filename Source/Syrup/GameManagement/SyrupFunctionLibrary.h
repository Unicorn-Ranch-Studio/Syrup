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
 * A class that holds functions that can be accessed globally
 */
UCLASS()
class SYRUP_API USyrupFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Gets the syrup player character
	 * 
	 * @param WorldContextObject - The world context object
	 * 
	 * @return The syrup player character
	 */
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static ASyrupPlayerCharacter* GetSyrupPlayerCharacter(const UObject* WorldContextObject);

};

/* /\ ===================== /\ *\
|  /\ USyrupFunctionLibrary /\  |
\* /\ ===================== /\ */