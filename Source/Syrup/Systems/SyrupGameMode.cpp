// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupGameMode.h"

#include "Kismet/GameplayStatics.h"

/* \/ ============== \/ *\
|  \/ ASyrupGameMode \/  |
\* \/ ============== \/ */

/* ----------------- *\
\* \/ Player Turn \/ */

/**
 * Ends the player's turn
 * 
 * @param WorldContextObject - An object in the same world as the player.
 */
void ASyrupGameMode::EndPlayerTurn(const UObject* WorldContextObject)
{
	ASyrupGameMode* GameMode = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	GameMode->BeginNight();
}

/* /\ Player Turn /\ *\
\* ----------------- */



/* --------------------- *\
\* \/ Effect Triggers \/ */

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

/**
 * Triggers a phase event for the world.
 *
 * @param TriggerType - The type of trigger to activate. Must be a phase event trigger.
 */
void ASyrupGameMode::TriggerPhaseEvent(const ETileEffectTriggerType TriggerType) const
{
	checkCode
	(
		if(TriggerType > LAST_PHASE_TRIGGER)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a Phase Event"), *StaticEnum<ETileEffectTriggerType>()->GetNameStringByValue((int64)TriggerType))
		}
	)

	TileEffectTriggerDelegate.Broadcast(TriggerType, TSet<FIntPoint>());
}

/* /\ Effect Triggers /\ *\
\* --------------------- */

/* /\ ============== /\ *\
|  /\ ASyrupGameMode /\  |
\* /\ ============== /\ */