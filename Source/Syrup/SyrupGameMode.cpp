// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupGameMode.h"

#include "Kismet/GameplayStatics.h"

/* \/ ============== \/ *\
|  \/ ASyrupGameMode \/  |
\* \/ ============== \/ */
/**
 * Gets the delegate used to bind and trigger tile effects.
 *
 * @param WorldContextObject - An object in the same world as the delegate.
 *
 * @return The delegate used to bind and trigger tile effects.
 */
FTileEffecTrigger& ASyrupGameMode::GetTileEffectTriggerDelegate(const UObject* WorldContextObject)
{
	ASyrupGameMode* GameMode = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	return GameMode->TileEffectTriggerDelegate;
}
/* /\ ============== /\ *\
|  /\ ASyrupGameMode /\  |
\* /\ ============== /\ */