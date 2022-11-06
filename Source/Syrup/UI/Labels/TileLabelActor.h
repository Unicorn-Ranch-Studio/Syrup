// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Syrup/Tiles/Effects/TileEffectTrigger.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileLabelActor.generated.h"

class UTileLabelContainer;
class UTileLabel;
class UWidgetComponent;

UCLASS(NotBlueprintable, HideDropdown)
class SYRUP_API ATileLabelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	 * Creates a tile label actor at the given tile location.
	 * 
	 * @param WorldContextObject - Some object in the world to create the label in.
	 * @param Location - The grid location to spawn the label actor at.
	 * @param ContainerClass - The class of container to create.
	 * 
	 * @return The label container at this grid location.
	 */
	UFUNCTION()
	static UTileLabelContainer* Create(const UObject* WorldContextObject, const TSubclassOf<UTileLabelContainer> ContainerClass, const FIntPoint Location);

	// Sets default values for this actor's properties
	ATileLabelActor();

private:
	/**
	 * Snaps this label actor to the tile at its location if it exists
	 */
	UFUNCTION()
	void Snap();

	/**
	 * Updates the label to snap/unsnap from tiles.
	 * 
	 * @param TriggerType - The of trigger that was activated.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
	 */
	UFUNCTION()
	void ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& LocationsToTrigger);

	UFUNCTION()
	void DestroyLabel();

	//The location this actor is labeling.
	UPROPERTY()
	FIntPoint GridLocation = FIntPoint::ZeroValue;

	//The widget component used to render the labels
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* WidgetComponent;
};
