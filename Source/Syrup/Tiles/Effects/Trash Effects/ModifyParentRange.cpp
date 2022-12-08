// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifyParentRange.h"

#include "Syrup/Tiles/Trash.h"
#include "Syrup/Tiles/Plant.h"


/* \/ ================== \/ *\
|  \/ UModifyParentRange \/  |
\* \/ ================== \/ */
/*
 * Causes this effect.
 *
 * @param Locations - The locations to effect.
 */
void UModifyParentRange::Affect(const TSet<FIntPoint>& Locations)
{
	ATrash* Trash = Cast<ATrash>(GetOwner());
	if (IsValid(Trash))
	{
		Trash->SetRange(Trash->GetRange() + DeltaRange);
		return;
	}

	APlant* Plant = Cast<APlant>(GetOwner());
	if (IsValid(Plant))
	{
		Plant->SetRange(Plant->GetRange() + DeltaRange);
	}
}
/* /\ ================== /\ *\
|  /\ UModifyParentRange /\  |
\* /\ ================== /\ */