// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TileLabelVisibility.h"

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
UCLASS(DefaultToInstanced, Abstract, EditInlineNew)
class SYRUP_API UTileLabel : public UUserWidget
{
	GENERATED_BODY()

public:	
	/**
	 * Creates a copy of this label at the given location.
	 * 
	 * @param WorldContextObject - An object in the same world as the one that copy should be made in.
	 * @param CopyLocation - The location of the new tile label.
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


	/**
	 * Whether or not this label is empty of data
	 *
	 * @return Whether or not any labels (including the initial label) have been merged into this.
	 */
	UFUNCTION(BlueprintPure)
	bool IsEmpty() const;

	//The locations of all the things creating this label.
	UPROPERTY(BlueprintReadOnly)
	TSet<FIntPoint> SourceLocations = TSet<FIntPoint>();

protected:
	//When this label will be visible.
	UPROPERTY(EditDefaultsOnly)
	ETileLabelVisibility LabelVisisbility;

	//The location being labeled.
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Location = FIntPoint::ZeroValue;

private:
	/**
	 * Binds the appropriate visibility events
	 */
	virtual void NativeConstruct() override;

	/**
	 * Unbinds the appropriate visibility events
	 */
	virtual void NativeDestruct() override;

	/**
	 * Sets the appropriate visibility of this given the new activation state.
	 */
	UFUNCTION()
	void OnTileLabelActivityChanged(bool bNowActive, FIntPoint NewLocation);

	//The total number of labels that have been merged into this.
	UPROPERTY()
	int MergeCount = 1;

	//The of labels merged into this with each source location.
	UPROPERTY()
	TMap<FIntPoint, int> SourceLocationsToCounts = TMap<FIntPoint, int>();
};
/* /\ ========== /\ *\
|  /\ UTileLabel /\  |
\* /\ ========== /\ */