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
	AffectTriggers.Add(ETileEffectTriggerType::TrashSpawned);
	UnaffectTriggers.Add(ETileEffectTriggerType::OnDeactivated);
	UnaffectTriggers.Add(ETileEffectTriggerType::TrashPickedUp);
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
		if (IsValid(Trash) && !EffectedTrash.Contains(Trash))
		{
			Trash->SetRange(Trash->GetRange() + DeltaRange);
			EffectedLocations.Add(Trash->GetGridTransform().Location);
			EffectedTrash.Add(Trash);
		}
	}
}

/*
 * Undoes this effect.
 *
 * @param Locations - The locations undo the effect in.
 */
void UModifyTrashRange::Unaffect(const TSet<FIntPoint>& Locations)
{
	TSet<FIntPoint> NewEffectedLocations = EffectedLocations.Difference(Locations);
	for (TSet<ATrash*>::TIterator Iterator = EffectedTrash.CreateIterator(); Iterator; ++Iterator)
	{
		if (IsValid(*Iterator) && (*Iterator)->GetSubTileLocations().Intersect(NewEffectedLocations).IsEmpty())
		{
			(*Iterator)->SetRange((*Iterator)->GetRange() - DeltaRange);
			Iterator.RemoveCurrent();
		}
	}
	EffectedLocations = NewEffectedLocations;
}
/* /\ ================= /\ *\
|  /\ UModifyTrashRange /\  |
\* /\ ================= /\ */