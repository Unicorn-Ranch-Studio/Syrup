// Fill out your copyright notice in the Description page of Project Settings.


#include "TileLabelContainer.h"

#include "TileLabel.h"

/* \/ =================== \/ *\
|  \/ UTileLabelContainer \/  |
\* \/ =================== \/ */
/**
 * Registers a label with the given payload so that it can be rendered by this. If label already exist the given payload will be added
 * to the current label's payload.
 *
 * @param Label - The label to render.
 */
void UTileLabelContainer::RegisterLabel(const UTileLabel* Label)
{
	UTileLabel* ExistingLabel = nullptr;
	if (!Labels.IsEmpty())
	{
		ExistingLabel = *Labels.FindByPredicate([Label](UTileLabel* EachLabel) { return EachLabel->GetClass() == Label->GetClass(); });
	}

	if (!IsValid(ExistingLabel))
	{
		ExistingLabel = Label->CreateCopy(GetWorld(), Location);
		Labels.Add(ExistingLabel);
		SetUpLabel(ExistingLabel);
	}
	else
	{
		ExistingLabel->MergeFrom(Label);
	}
}

/**
 * Unregisters the label with the given payload so that it is no longer rendered by this. If this type of label has been registered
 * multiple times, then the given payload will be removed from the current label's payload.
 *
 * @param Label - The label to unregister.
 */
void UTileLabelContainer::UnregisterLabel(const UTileLabel* Label)
{
	UTileLabel* ExistingLabel = *Labels.FindByPredicate([Label](UTileLabel* EachLabel) { return EachLabel->GetClass() == Label->GetClass(); });
	if (IsValid(ExistingLabel))
	{
		ExistingLabel->SplitFrom(Label);
	}
}

/* /\ =================== /\ *\
|  /\ UTileLabelContainer /\  |
\* /\ =================== /\ */