// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TileLabel.generated.h"

class UTileLabelPayload;

/**
 * 
 */
UCLASS()
class SYRUP_API UTileLabel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * Creates a tile label with the given payload.
	 * 
	 * @param Payload -  The payload to give the created label.
	 * 
	 * @return The label that was created.
	 */
	UFUNCTION()
	static UTileLabel* Create(UTileLabelPayload* Payload);

	
	/**
	 * Creates a tile label with the given payload.
	 */
	UFUNCTION()
	void AddPayload(UTileLabelPayload* Payload);

	UFUNCTION()
	void RemovePayload(UTileLabelPayload* Payload);

private:
	UPROPERTY()
	TArray<UTileLabelPayload*> Payloads;
};
