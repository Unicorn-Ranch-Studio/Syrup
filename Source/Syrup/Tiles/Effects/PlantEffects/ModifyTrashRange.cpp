// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifyTrashRange.h"

#include "Syrup/Tiles/Trash.h"

/* \/ ================= \/ *\
|  \/ UModifyTrashRange \/  |
\* \/ ================= \/ */
/**
 * Overrides the triggers variable.
 */
UModifyTrashRange::UModifyTrashRange()
{
	AffectTriggers.Add(ETileEffectTriggerType::OnActivated);
	UnaffectTriggers.Add(ETileEffectTriggerType::OnDeactivated);
}

/*
 * Causes this effect.
 *
 * @param Locations - The locations to effect.
 */
void UModifyTrashRange::Affect(const TSet<FIntPoint>& Locations)
{
	TSet<ATile*> EffectTiles;
	UGridLibrary::OverlapShape(GetWorld(), Locations, EffectTiles, TArray<AActor*>());

	for (ATile* EachEffectedTile : EffectTiles)
	{
		ATrash* Trash = Cast<ATrash>(EachEffectedTile);
		if (IsValid(Trash))
		{
			Trash->SetRange(Trash->GetRange() + DeltaRange);
			EffectedLocations.Add(Trash->GetGridTransform().Location);
		}
	}
}
/* /\ ================= /\ *\
|  /\ UModifyTrashRange /\  |
\* /\ ================= /\ */