// Fill out your copyright notice in the Description page of Project Settings.


#include "TileLabel.h"

DEFINE_LOG_CATEGORY(LogLabel);

/* \/ ========== \/ *\
|  \/ UTileLabel \/  |
\* \/ ========== \/ */

/**
 * Creates a copy of this label at the given location.
 *
 * @param WorldContextObject - An object in the same world as the one that copy should be made in.
 * @param CopyLocation - The location of the new tile.
 *
 * @return The copy that was created.
 */
UTileLabel* UTileLabel::CreateCopy_Implementation(const UObject* WorldContextObject, const FIntPoint CopyLocation) const
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

/* /\ ========== /\ *\
|  /\ UTileLabel /\  |
\* /\ ========== /\ */