// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Syrup/Tiles/Effects/TileEffectTrigger.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SyrupGameMode.generated.h"

class UTileLabel;
class UTileLabelContainer;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTileLabelActivityUpdate, bool, bNowActive, FIntPoint, NewLocation);

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

	ASyrupGameMode();

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
	static FTileEffectTrigger& GetTileEffectTriggerDelegate(const UObject* WorldContextObject);

	UPROPERTY(BlueprintAssignable)
	FTileEffectTrigger TileEffectTriggerDelegate;

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
	 * @param WorldContextObject - An object in the same world as the label.
	 * @param Location - A location relating to the tile labels to activate.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI", Meta = (WorldContext = "WorldContextObject"))
	static void ActivateLabels(const UObject* WorldContextObject, const FIntPoint Location);

	/**
	 * Makes all the tile labels relating to the given area deactivate.
	 *
	 * @param WorldContextObject - An object in the same world as the label.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI", Meta = (WorldContext = "WorldContextObject"))
	static void DeactivateLabels(const UObject* WorldContextObject);

	/**
	 * Makes all the tile labels relating to the given area activate. Also deactivates he previously active labels.
	 * 
	 * @param WorldContextObject - An object in the same world as the label.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI", Meta = (WorldContext = "WorldContextObject"))
	static FIntPoint GetActiveLabelLocation(const UObject* WorldContextObject);

	/**
	 * Registers a tile label at the given location so that it may be rendered when the appropriate locations are selected.
	 *
	 * @param WorldContextObject - An object in the same world as the label.
	 * @param Label - The label to render.
	 * @param Location - The location being labeled.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI", Meta = (WorldContext = "WorldContextObject"))
	static void RegisterTileLabel(const UObject* WorldContextObject, UTileLabel* Label, const FIntPoint Location);

	/**
	 * Unregisters a tile label at the given location so that it is no longer able to be rendered.
	 *
	 * @param WorldContextObject - An object in the same world as the label.
	 * @param Label - The label to unregister.
	 * @param Location - The location being unlabeled.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI", Meta = (WorldContext = "WorldContextObject"))
	static void UnregisterTileLabel(const UObject* WorldContextObject, const UTileLabel* Label, const FIntPoint Location);

	/**
	 * Gets the delegate used to bind and trigger tile label activation change effects.
	 * 
	 * @param WorldContextObject - An object in the same world as the delegate.
	 * 
	 * @return The delegate used to bind and trigger tile label activation change effects.
	 */
	UFUNCTION()
	static FTileLabelActivityUpdate& GetOnActiveLabelChangedDelegate(const UObject* WorldContextObject);

	//Called when a label is either activated or deactivated.
	UPROPERTY(BlueprintAssignable)
	FTileLabelActivityUpdate OnActiveLabelChanged;
private:
	//Stores the tile label container at each location.
	UPROPERTY()
	TMap<FIntPoint, UTileLabelContainer*> LocationsToLabelConatiners = TMap<FIntPoint, UTileLabelContainer*>();

	//Whether or not a label is curenly active.
	UPROPERTY()
	bool bLabelActive = false;

	//The currently active label's location.
	UPROPERTY()
	FIntPoint ActiveLabelLocation = FIntPoint::ZeroValue;

	//The blueprint TileLabelContainer class.
	UPROPERTY(EditAnywhere, Category = "Classes")
	TSubclassOf<UTileLabelContainer> TileLabelContainerClass;

	/* /\ UI /\ *\
	\* -------- */
};

/* /\ ============== /\ *\
|  /\ ASyrupGameMode /\  |
\* /\ ============== /\ */
