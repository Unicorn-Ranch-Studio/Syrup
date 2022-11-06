// Fill out your copyright notice in the Description page of Project Settings.


#include "TileLabelContainer.h"
#include "Components/PanelWidget.h"

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
void UTileLabelContainer::RegisterLabel(UTileLabel* Label)
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
	if (!Labels.IsEmpty())
	{
		UTileLabel* ExistingLabel = *Labels.FindByPredicate([Label](UTileLabel* EachLabel) { return EachLabel->GetClass() == Label->GetClass(); });
		if (IsValid(ExistingLabel))
		{
			UE_LOG(LogTemp, Warning, TEXT("Split"))
			ExistingLabel->SplitFrom(Label);
			if (ExistingLabel->IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("Maby should desroy"))
				Labels.Remove(ExistingLabel);
				if (Labels.IsEmpty())
				{
					UE_LOG(LogTemp, Warning, TEXT("Should Destroy"))
					OnContainerEmptied.Broadcast();
				}
			}
		}
	}
}

/**
 * Updates a label inside this container.
 *
 * @param PrevousLabel - The old label whose values are to overridden.
 * @param Label - The new label to whose values are to be copied.
 */
void UTileLabelContainer::UpdateLabel(const UTileLabel* PrevousLabel, const UTileLabel* Label)
{
	if (ensure(!Labels.IsEmpty()))
	{
		UTileLabel* ExistingLabel = *Labels.FindByPredicate([Label](UTileLabel* EachLabel) { return EachLabel->GetClass() == Label->GetClass(); });
		if (ensure(IsValid(ExistingLabel)))
		{
			ExistingLabel->SplitFrom(PrevousLabel);
			if (ExistingLabel->IsEmpty())
			{
				SetUpLabel(ExistingLabel);
			}
			ExistingLabel->MergeFrom(Label);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Test %i"), Labels.IsEmpty() ? 1:2)
		}
	}
}


/**
 * Whether or not this is empty of labels.
 *
 * @return Whether or not this does not contains labels.
 */
bool UTileLabelContainer::IsEmpty() const
{
	return Labels.IsEmpty();
}
/* /\ =================== /\ *\
|  /\ UTileLabelContainer /\  |
\* /\ =================== /\ */