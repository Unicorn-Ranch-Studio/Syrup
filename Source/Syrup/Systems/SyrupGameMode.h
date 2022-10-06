// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Syrup/Tiles/Effects/TileEffectTrigger.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SyrupGameMode.generated.h"

class UTileLabel;

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



	/* -------- *\
	\* \/ UI \/ */
	
public:

	/**
	 * Makes all the tile labels relating to the given area activate. Also deactivates he previously active labels.
	 * 
	 * @param Location - A location relating to the tile labels to activate.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	static void ActivateLabels(const FIntPoint Location);

	/**
	 * Makes all the tile labels relating to the given area deactivate.
	 *
	 * @param Location - A location relating to the tile labels to deactivate.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	static void DeactivateLabels(const FIntPoint Location);

	/**
	 * Registers a tile label at the given location so that it may be rendered when the appropriate locations are selected.
	 *
	 * @param SourceLocation - The location of the thing creating the label.
	 * @param LabelLocation - The location being labeled.
	 * @param LabelType - The type of label to render.
	 * @param LabelPayload - The value the label should try to render.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	static void RegisterTileLabel(const FIntPoint SourceLocation, const FIntPoint LabelLocation, const TSubclassOf<UTileLabel> LabelType, const int LabelPayload = 0);

	/**
	 * Unregisters a tile label at the given location so that it is no longer able to be rendered.
	 *
	 * @param SourceLocation - The location of the thing unregistering the label.
	 * @param LabelLocation - The location to remove the label from labeled.
	 * @param LabelType - The type of label to unregister.
	 * @param LabelPayload - The payload of the label to unregister.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	static void UnregisterTileLabel(const FIntPoint SourceLocation, const FIntPoint LabelLocation, const TSubclassOf<UTileLabel> LabelType, const int LabelPayload = 0);
	
	/* /\ UI /\ *\
	\* -------- */
};

/* /\ ============== /\ *\
|  /\ ASyrupGameMode /\  |
\* /\ ============== /\ */
