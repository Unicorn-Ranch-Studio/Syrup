// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TileLabelContainer.generated.h"

class UTileLabel;
class UTileLabelPayload;

//UDELEGATE()
DECLARE_MULTICAST_DELEGATE(FEmptyedDelegate);

/* \/ =================== \/ *\
|  \/ UTileLabelContainer \/  |
\* \/ =================== \/ */
/**
 * Handles the rendering of multiple tile labels all for the same tile.
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
	 */
	UFUNCTION()
	void RegisterLabel(UTileLabel* Label);

	/**
	 * Unregisters the label with the given payload so that it is no longer rendered by this. If this type of label has been registered 
	 * multiple times, then the given payload will be removed from the current label's payload.
	 * 
	 * @param Label - The label to unregister.
	 */
	UFUNCTION()
	void UnregisterLabel(const UTileLabel* Label);
	
	/**
	 * Updates a label inside this container.
	 *
	 * @param PrevousLabel - The old label whose values are to overridden.
	 * @param Label - The new label to whose values are to be copied.
	 */
	UFUNCTION()
	void UpdateLabel(const UTileLabel* PrevousLabel, const UTileLabel* Label);

	/**
	 * Whether or not this is empty of labels.
	 *
	 * @return Whether or not this does not contains labels.
	 */
	UFUNCTION(BlueprintPure)
	bool IsEmpty() const;

	//The location of this container.
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Location = FIntPoint::ZeroValue;

	//Called when this container is emptied.
	FEmptyedDelegate OnContainerEmptied;

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
	TArray<UTileLabel*> Labels = TArray<UTileLabel*>();
};

/* /\ =================== /\ *\
|  /\ UTileLabelContainer /\  |
\* /\ =================== /\ */