// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifyTrashDamage.h"

#include "Syrup/Tiles/Trash.h"

/* \/ ================== \/ *\
|  \/ UModifyTrashDamage \/  |
\* \/ ================== \/ */
/**
 * Overrides the triggers variable.
 */
UModifyTrashDamage::UModifyTrashDamage()
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
void UModifyTrashDamage::Affect(const TSet<FIntPoint>& Locations)
{
	TSet<ATile*> EffectTiles;
	TArray<AActor*> IgnoredActors = TArray<AActor*>();
	if(!bEffectParent)
	{
		IgnoredActors.Add(GetOwner());
	}

	UGridLibrary::OverlapShape(GetWorld(), Locations, EffectTiles, IgnoredActors);

	for (ATile* EachEffectedTile : EffectTiles)
	{
		ATrash* Trash = Cast<ATrash>(EachEffectedTile);
		if (IsValid(Trash) && !EffectedTrash.Contains(Trash))
		{
			Trash->SetDamage(Trash->GetDamage() + DeltaDamage);
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
void UModifyTrashDamage::Unaffect(const TSet<FIntPoint>& Locations)
{
	TSet<FIntPoint> NewEffectedLocations = EffectedLocations.Difference(Locations);
	for (TSet<ATrash*>::TIterator Iterator = EffectedTrash.CreateIterator(); Iterator; ++Iterator)
	{
		if (IsValid(*Iterator) && (*Iterator)->GetSubTileLocations().Intersect(NewEffectedLocations).IsEmpty())
		{
			(*Iterator)->SetDamage((*Iterator)->GetDamage() - DeltaDamage);
			Iterator.RemoveCurrent();
		}
	}
	EffectedLocations = NewEffectedLocations;
}
/* /\ ================== /\ *\
|  /\ UModifyTrashDamage /\  |
\* /\ ================== /\ */