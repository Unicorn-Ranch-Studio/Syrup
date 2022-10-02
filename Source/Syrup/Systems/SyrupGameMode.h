// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Syrup/Tiles/Effects/TileEffectTrigger.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SyrupGameMode.generated.h"


/* \/ ============== \/ *\
|  \/ ASyrupGameMode \/  |
\* \/ ============== \/ */
/**
 * The base game mode for Syrup. Handles the turn system and triggering tile effects.
 */
UCLASS(Blueprintable)
class SYRUP_API ASyrupGameMode : public AGameModeBase
{
	GENERATED_BODY()


	/* ----------------- *\
	\* \/ Player Turn \/ */

public:
	
	/**
	 * Ends the player's turn
	 * 
	 * @param WorldContextObject - An object in the same world as the player.
	 */
	UFUNCTION(BlueprintCallable, Category = "Player Turn", Meta = (WorldContext = "WorldContextObject"))
	static void EndPlayerTurn(const UObject* WorldContextObject);

protected:

	/**
	 * Causes night to fall and non-player objects to take their turns.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Player Turn")
	void BeginNight();

	/* /\ Player Turn /\ *\
	\* ----------------- */



	/* --------------------- *\
	\* \/ Effect Triggers \/ */

public:

	/**
	 * Gets the delegate used to bind and trigger tile effects.
	 * 
	 * @param WorldContextObject - An object in the same world as the delegate.
	 * 
	 * @return The delegate used to bind and trigger tile effects.
	 */
	UFUNCTION()
	static FTileEffecTrigger& GetTileEffectTriggerDelegate(const UObject* WorldContextObject);

	UPROPERTY(BlueprintAssignable)
	FTileEffecTrigger TileEffectTriggerDelegate;

protected:
	/**
	 * Triggers a phase event for the world.
	 * 
	 * @param TriggerType - The type of trigger to activate. Must be a phase event trigger.
	 */
	UFUNCTION(BlueprintCallable)
	void TriggerPhaseEvent(const ETileEffectTriggerType TriggerType) const;

	/* /\ Effect Triggers /\ *\
	\* --------------------- */
};

/* /\ ============== /\ *\
|  /\ ASyrupGameMode /\  |
\* /\ ============== /\ */
