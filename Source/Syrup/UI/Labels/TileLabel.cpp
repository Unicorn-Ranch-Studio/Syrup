// Fill out your copyright notice in the Description page of Project Settings.


#include "TileLabel.h"

#include "Syrup/Systems/SyrupGameMode.h"

DEFINE_LOG_CATEGORY(LogLabel);

/* \/ ========== \/ *\
|  \/ UTileLabel \/  |
\* \/ ========== \/ */

/**
 * Creates a copy of this label at the given location.
 *
 * @param WorldContextObject - An object in the same world as the one that copy should be made in.
 * @param CopyLocation - The location of the new tile label.
 *
 * @return The copy that was created.
 */
UTileLabel* UTileLabel::CreateCopy_Implementation(const UObject* WorldContextObject, const FIntPoint CopyLocation)
{
	UTileLabel* Copy = CreateWidget<UTileLabel>(WorldContextObject->GetWorld(), GetClass());
	Copy->SourceLocations = SourceLocations;
	Copy->Location = CopyLocation;
	return Copy;
}

/**
 * Adds the attributes of another label to this.
 *
 * @param Other - The label who's attributes will be added to this.
 */
void UTileLabel::MergeFrom_Implementation(const UTileLabel* Other)
{
	MergeCount++;
	for (FIntPoint EachSourceLocation : Other->SourceLocations)
	{
		if (SourceLocations.Contains(EachSourceLocation))
		{
			SourceLocationsToCounts.Add(EachSourceLocation, SourceLocationsToCounts.FindRef(EachSourceLocation) + 1);
		}
		else
		{
			SourceLocations.Add(EachSourceLocation);
			SourceLocationsToCounts.Add(EachSourceLocation, 1);
		}
	}
}

/**
 * Removes the attributes of this from another label.
 *
 * @param Other - The label to remove the attributes of this from.
 */
void UTileLabel::SplitFrom_Implementation(const UTileLabel* Other)
{
	if (--MergeCount > 0)
	{
		for (FIntPoint EachSourceLocation : Other->SourceLocations)
		{
			if (SourceLocationsToCounts.FindRef(EachSourceLocation) <= 1)
			{
				SourceLocations.Remove(EachSourceLocation);
				SourceLocationsToCounts.Remove(EachSourceLocation);
			}
			else
			{
				SourceLocationsToCounts.Add(EachSourceLocation, SourceLocationsToCounts.FindRef(EachSourceLocation) - 1);
			}
		}
	}
	else
	{
		RemoveFromParent();
	}
}

/**
 * Whether or not this label is empty of data
 *
 * @return Whether or not any labels (including the initial label) have been merged into this.
 */
bool UTileLabel::IsEmpty() const
{
	return MergeCount <= 0;
}

/**
 * Binds the appropriate visibility events
 */
void UTileLabel::NativeConstruct()
{
	if (LabelVisisbility != ETileLabelVisibility::Never || LabelVisisbility != ETileLabelVisibility::Always)
	{
		ASyrupGameMode::GetOnActiveLabelChangedDelegate(this).AddDynamic(this, &UTileLabel::OnTileLabelActivityChanged);
	}
}

/**
 * Unbinds the appropriate visibility events
 */
void UTileLabel::NativeDestruct()
{
	if (LabelVisisbility != ETileLabelVisibility::Never || LabelVisisbility != ETileLabelVisibility::Always)
	{
		ASyrupGameMode::GetOnActiveLabelChangedDelegate(this).RemoveDynamic(this, &UTileLabel::OnTileLabelActivityChanged);
	}
}

/**
 * Sets the appropriate visibility of this given the new activation state.
 */
void UTileLabel::OnTileLabelActivityChanged(bool bNowActive, FIntPoint NewLocation)
{
	if (NewLocation == Location)
	{
		if (LabelVisisbility == ETileLabelVisibility::LocationOrSource || LabelVisisbility == ETileLabelVisibility::Location)
		{
			SetVisibility(bNowActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
	} 
	else if (SourceLocations.Contains(NewLocation))
	{
		if (LabelVisisbility == ETileLabelVisibility::LocationOrSource || LabelVisisbility == ETileLabelVisibility::Source)
		{
			SetVisibility(bNowActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
	}
}

/* /\ ========== /\ *\
|  /\ UTileLabel /\  |
\* /\ ========== /\ */