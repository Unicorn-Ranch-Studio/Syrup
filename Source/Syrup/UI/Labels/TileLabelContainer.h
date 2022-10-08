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
	 * @param Label - The label to render.
	 * @
	 */
	UFUNCTION()
	void RegisterLabel(const UTileLabel* Label);

	/**
	 * Unregisters the label with the given payload so that it is no longer rendered by this. If this type of label has been registered 
	 * multiple times, then the given payload will be removed from the current label's payload.
	 * 
	 * @param Label - The label to unregister.
	 */
	UFUNCTION()
	void UnregisterLabel(const UTileLabel* Label);
	
	//The location of this container.
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Location = FIntPoint::ZeroValue;
protected:
	/**
	 * Sets up a label so it can be rendered.
	 * 
	 * @param NewLabel - The label that needs setting up
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetUpLabel(UTileLabel* NewLabel);

	//The labels that need to be rendered.
	UPROPERTY(BlueprintReadOnly)
	TArray<UTileLabel*> Labels;
};
