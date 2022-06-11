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
 * @return The syrup player character
 */
ASyrupPlayerCharacter* USyrupFunctionLibrary::GetSyrupPlayerCharacter()
{
	return Cast<ASyrupPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}


/* /\ ===================== /\ *\
|  /\ USyrupFunctionLibrary /\  |
\* /\ ===================== /\ */