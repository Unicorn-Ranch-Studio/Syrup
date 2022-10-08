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
	 * @param CopyLocation - The location of the new tile.
	 * 
	 * @return The copy that was created.
	 */
	UFUNCTION(BlueprintNativeEvent)
	UTileLabel* CreateCopy(const FIntPoint CopyLocation) const;
	UTileLabel* CreateCopy_Implementation(const FIntPoint CopyLocation) const;

	/**
	 * Adds the attributes of this to another label.
	 * 
	 * @param Other - The label to add the attributes of this to.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void MergeInto(const UTileLabel* Other) const;
	void MergeInto_Implementation(const UTileLabel* Other) const;

	/**
	 * Removes the attributes of this from another label.
	 * 
	 * @param Other - The label to remove the attributes of this from.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void SplitFrom(const UTileLabel* Other) const;
	void SplitFrom_Implementation(const UTileLabel* Other) const;

protected:
	//The locations of all the things creating this label.
	UPROPERTY(BlueprintReadOnly)
	TSet<FIntPoint> SourceLocations = TSet<FIntPoint>();

	//The location being labeled.
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Location = FIntPoint::ZeroValue;
};
