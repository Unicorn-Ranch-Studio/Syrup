// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupGameMode.h"

#include "Syrup/UI/Labels/TileLabelContainer.h"
#include "Syrup/UI/Labels/TileLabel.h"
#include "Syrup/UI/Labels/TileLabelActor.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

/* \/ ============== \/ *\
|  \/ ASyrupGameMode \/  |
\* \/ ============== \/ */

ASyrupGameMode::ASyrupGameMode()
{
	static ConstructorHelpers::FClassFinder<UTileLabelContainer> MyWidgetClass(TEXT("/Game/UI/TileLabels/WBP_TileLabelContianer"));
	TileLabelContainerClass = MyWidgetClass.Class;
}

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
	if (GameMode->bIsPlayerTurn)
	{
		GameMode->BeginNight();
		GameMode->bIsPlayerTurn = false;
	}
}


/**
 * Gets whether or not it is currently the player's turn.
 *
 * @param WorldContextObject - An object in the same world as the player.
 *
 * @return If it is the player's turn.
 */
bool ASyrupGameMode::IsPlayerTurn(const UObject* WorldContextObject)
{
	ASyrupGameMode* GameMode = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	return GameMode->bIsPlayerTurn;

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
FTileEffectTrigger& ASyrupGameMode::GetTileEffectTriggerDelegate(const UObject* WorldContextObject)
{
	ASyrupGameMode* GameMode = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	return GameMode->TileEffectTriggerDelegate;
}

/**
 * Triggers a phase event for the world.
 *
 * @param TriggerType - The type of trigger to activate. Must be a phase event trigger.
 */
void ASyrupGameMode::TriggerPhaseEvent(const ETileEffectTriggerType TriggerType)
{
	checkCode
	(
		if(TriggerType > LAST_PHASE_TRIGGER)
		{
			UE_LOG(LogTemp, Error, TEXT("%s is not a Phase Event"), *StaticEnum<ETileEffectTriggerType>()->GetNameStringByValue((int64)TriggerType))
			return;
		}
	)

	TileEffectTriggerDelegate.Broadcast(TriggerType, nullptr, TSet<FIntPoint>());

	if (TriggerType == LAST_PHASE_TRIGGER)
	{
		bIsPlayerTurn = true;
	}
}

/* /\ Effect Triggers /\ *\
\* --------------------- */



/* -------- *\
\* \/ UI \/ */

/**
 * Makes all the tile labels relating to the given area activate. Also deactivates he previously active labels.
 *
 * @param WorldContextObject - An object in the same world as the label.
 * @param Location - A location relating to the tile labels to activate.
 */
void ASyrupGameMode::ActivateLabels(const UObject* WorldContextObject, const FIntPoint Location)
{
	ASyrupGameMode* GameMode = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	if (GameMode->bLabelActive)
	{
		DeactivateLabels(WorldContextObject);
	}

	GameMode->bLabelActive = true;
	GameMode->ActiveLabelLocation = Location;
	GameMode->OnActiveLabelChanged.Broadcast(true, Location);
}

/**
 * Makes all the tile labels relating to the given area deactivate.
 *
 * @param WorldContextObject - An object in the same world as the label.
 */
void ASyrupGameMode::DeactivateLabels(const UObject* WorldContextObject)
{
	ASyrupGameMode* GameMode = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	GameMode->bLabelActive = false;
	GameMode->OnActiveLabelChanged.Broadcast(false, GameMode->ActiveLabelLocation);
	GameMode->ActiveLabelLocation = FIntPoint::ZeroValue;
}

/**
 * Makes all the tile labels relating to the given area activate. Also deactivates he previously active labels.
 *
 * @param WorldContextObject - An object in the same world as the label.
 */
FIntPoint ASyrupGameMode::GetActiveLabelLocation(const UObject* WorldContextObject)
{
	return Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject))->ActiveLabelLocation;
}

/**
 * Registers a tile label at the given location so that it may be rendered when the appropriate locations are selected.
 *
 * @param WorldContextObject - An object in the same world as the label.
 * @param Label - The label to render.
 * @param Location - The location being labeled.
 */
void ASyrupGameMode::RegisterTileLabel(const UObject* WorldContextObject, UTileLabel* Label, const FIntPoint Location)
{
	if (IsValid(Label))
	{
		ASyrupGameMode* GameMode = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
		UTileLabelContainer* LabelContainer = GameMode->LocationsToLabelConatiners.FindRef(Location);
		if (!IsValid(LabelContainer))
		{
			LabelContainer = ATileLabelActor::Create(WorldContextObject, GameMode->TileLabelContainerClass, Location);

			GameMode->LocationsToLabelConatiners.Add(Location, LabelContainer);
		}

		LabelContainer->RegisterLabel(Label);
	}
	else
	{
		UE_LOG(LogLabel, Error, TEXT("Can't registered label. Label is null."))
	}
}

/**
 * Unregisters a tile label at the given location so that it is no longer able to be rendered.
 *
 * @param WorldContextObject - An object in the same world as the label.
 * @param Label - The label to unregister.
 * @param Location - The location being unlabeled.
 */
void ASyrupGameMode::UnregisterTileLabel(const UObject* WorldContextObject, const UTileLabel* Label, const FIntPoint Location)
{
	if (IsValid(Label))
	{
		ASyrupGameMode* GameMode = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
		if (IsValid(GameMode))
		{
			UTileLabelContainer* LabelContainer = GameMode->LocationsToLabelConatiners.FindRef(Location);
			if (IsValid(LabelContainer))
			{
				LabelContainer->UnregisterLabel(Label);
				if (LabelContainer->IsEmpty())
				{
					GameMode->LocationsToLabelConatiners.Remove(Location);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogLabel, Error, TEXT("Can't unregistered label. Label is null."))
	}
}

/**
 * Updates a tile label.
 *
 * @param WorldContextObject - An object in the same world as the label.
 * @param PreviousLabel - The old label being rendered.
 * @param Label - The new label to render.
 * @param Location - The location being labeled.
 */
void ASyrupGameMode::UpdateTileLabel(const UObject* WorldContextObject, UTileLabel* PreviousLabel, UTileLabel* Label, const FIntPoint Location)
{
	ASyrupGameMode* GameMode = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (IsValid(Label) && IsValid(PreviousLabel) && GameMode->LocationsToLabelConatiners.Contains(Location))
	{
		UTileLabelContainer* LabelContainer = GameMode->LocationsToLabelConatiners.FindRef(Location);
		if (ensure(IsValid(LabelContainer)))
		{
			LabelContainer->UpdateLabel(PreviousLabel, Label);
		}
	}
	else
	{
		UE_LOG(LogLabel, Error, TEXT("Can't update label. Label is null."))
	}
}

/**
 * Gets the delegate used to bind and trigger tile label activation change effects.
 *
 * @param WorldContextObject - An object in the same world as the delegate.
 *
 * @return The delegate used to bind and trigger tile label activation change effects.
 */
FTileLabelActivityUpdate& ASyrupGameMode::GetOnActiveLabelChangedDelegate(const UObject* WorldContextObject)
{
	ASyrupGameMode* GameMode = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	return GameMode->OnActiveLabelChanged;
}

/* /\ UI /\ *\
\* -------- */

/* /\ ============== /\ *\
|  /\ ASyrupGameMode /\  |
\* /\ ============== /\ */