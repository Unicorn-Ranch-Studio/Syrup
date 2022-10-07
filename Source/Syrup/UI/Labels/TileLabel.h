// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TileLabel.generated.h"

/**
 * A widget that labels some tile with some information.
 */
UCLASS(Abstract, EditInlineNew)
class SYRUP_API UTileLabel : public UUserWidget
{
	GENERATED_BODY()

public:	
	/**
	 * Creates a copy of this label at the given location.
	 * 
	 * @param Location - The location of the new tile.
	 * 
	 * @return The copy that was created.
	 */
	UFUNCTION(BlueprintNativeEvent)
	UTileLabel* Copy(const FIntPoint Location) const;
	UTileLabel* Copy_Implementation(const FIntPoint Location) const;

	/**
	 * Adds the attributes of this to another label.
	 * 
	 * @param Other - The label to add the attributes of this to.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Merge(const UTileLabel* Other) const;
	void Merge_Implementation(const UTileLabel* Other) const;

	/**
	 * Removes the attributes of this from another label.
	 * 
	 * @param Other - The label to remove the attributes of this from.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Split(const UTileLabel* Other) const;
	void Split_Implementation(const UTileLabel* Other) const;

protected:
	//The location of the thing creating the label.
	UPROPERTY(BlueprintReadOnly)
	FIntPoint SourceLocation = FIntPoint::ZeroValue;

	//The location being labeled.
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Location = FIntPoint::ZeroValue;
};
