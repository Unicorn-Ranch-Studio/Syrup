// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TileLabelPayload.generated.h"

/**
 * An object for storing the data relating to what a tile label needs to render.
 */
UCLASS()
class SYRUP_API UTileLabelPayload : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Creates a payload with the given source and location
	 * 
	 * @param LabelSourceLocation - The location of the thing creating the label.
	 * @param LabelLocation - The location being labeled.
	 */
	UFUNTION()
	static UTileLabelPayload* Create(TSubclassOf<UTileLabelPayload> Class, FIntPoint LabelSourceLocation, FIntPoint LabelLocation);

	//The location of the thing creating the label.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FIntPoint SourceLocation = FIntPoint::ZeroValue;

	//The location being labeled.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FIntPoint Location = FIntPoint::ZeroValue;
};
