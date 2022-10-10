// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TileLabel.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLabel, Log, All);

/* \/ ========== \/ *\
|  \/ UTileLabel \/  |
\* \/ ========== \/ */
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
	 * @param WorldContextObject - An object in the same world as the one that copy should be made in.
	 * @param CopyLocation - The location of the new tile.
	 * 
	 * @return The copy that was created.
	 */
	UFUNCTION(BlueprintNativeEvent, Meta = (WorldContext = "WorldContextObject"))
	UTileLabel* CreateCopy(const UObject* WorldContextObject, const FIntPoint CopyLocation);
	UTileLabel* CreateCopy_Implementation(const UObject* WorldContextObject, const FIntPoint CopyLocation);

	/**
	 * Adds the attributes of another label to this.
	 *
	 * @param Other - The label who's attributes will be added to this.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void MergeFrom(const UTileLabel* Other);
	void MergeFrom_Implementation(const UTileLabel* Other);

	/**
	 * Removes the attributes of another label from this.
	 * 
	 * @param Other - The label to remove the attributes of.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void SplitFrom(const UTileLabel* Other);
	void SplitFrom_Implementation(const UTileLabel* Other);

protected:
	//The locations of all the things creating this label.
	UPROPERTY(BlueprintReadOnly)
	TSet<FIntPoint> SourceLocations = TSet<FIntPoint>();

	//The location being labeled.
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Location = FIntPoint::ZeroValue;

private:
	//The total number of labels that have been merged into this.
	UPROPERTY()
	int MergeCount = 0;

	//The of labels merged into this with each source location.
	UPROPERTY()
	TMap<FIntPoint, int> SourceLocationsToCounts = TMap<FIntPoint, int>();
};
/* /\ ========== /\ *\
|  /\ UTileLabel /\  |
\* /\ ========== /\ */