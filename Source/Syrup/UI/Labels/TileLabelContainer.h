// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TileLabelContainer.generated.h"

class UTileLabel;
class UTileLabelPayload;

/**
 * 
 */
UCLASS()
class SYRUP_API UTileLabelContainer : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/**
	 * Registers a label with the given payload so that it can be rendered by this. If label already exist the given payload will be added
	 * to the current label's payload.
	 * 
	 * @param LabelType - The type of label to register.
	 * @param LabelPayload - The payload of the label.
	 */
	UFUNCTION()
	void RegisterLabel(const TSubclassOf<UTileLabel> LabelType, const UTileLabelPayload* LabelPayload);

	/**
	 * Unregisters the label with the given payload so that it is no longer rendered by this. If this type of label has been registered 
	 * multiple times, then the given payload will be removed from the current label's payload.
	 * 
	 * @param LabelType - The type of label to unregister.
	 * @param LabelPayload - The payload of the label.
	 */
	UFUNCTION()
	void UnregisterLabel(const TSubclassOf<UTileLabel> LabelType, const UTileLabelPayload* LabelPayload);

protected:
	/**
	 * Sets up a label so it can be rendered.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void LabelCreated(UTileLabel* NewLabel);

	//The labels that need to be rendered.
	UPROPERTY(BlueprintReadOnly)
	TArray<UTileLabel*> Labels;
};
