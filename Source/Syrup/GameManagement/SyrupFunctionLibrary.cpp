// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SyrupPlayerCharacter.h"

/* \/ ====================== \/ *\
|  \/ USyrupFunctionLibrary  \/  |
\* \/ ====================== \/ */

/**
 * Gets the syrup player character
 * 
 * @param WorldContextObject - The world context object
 * 
 * @return The syrup player character
 */
ASyrupPlayerCharacter* USyrupFunctionLibrary::GetSyrupPlayerCharacter(const UObject* WorldContextObject)
{
	return Cast<ASyrupPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, 0));
}


/* /\ ===================== /\ *\
|  /\ USyrupFunctionLibrary /\  |
\* /\ ===================== /\ */