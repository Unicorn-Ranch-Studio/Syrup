// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyField.h"

#include "Syrup/Tiles/Tile.h"
#include "EngineUtils.h"

/* \/ =========== \/ *\
|  \/ UApplyField \/  |
\* \/ =========== \/ */

/**
 * Overrides the triggers variable.
 */
UApplyField::UApplyField()
{
	AffectTriggers.Add(ETileEffectTriggerType::OnActivated);
	AffectTriggers.Add(ETileEffectTriggerType::PlantSpawned);
	AffectTriggers.Add(ETileEffectTriggerType::TrashSpawned);
	UnaffectTriggers.Add(ETileEffectTriggerType::OnDeactivated);
}

/*
 * Causes this effect.
 *
 * @param Locations - The locations to effect.
 */
void UApplyField::Affect(const TSet<FIntPoint>& Locations)
{
	TSet<FIntPoint> NewlyEffectedLocations = Locations.Difference(EffectedLocations);

	//If no ground planes are known
	if (EffectedGroundPlanes.IsEmpty())
	{
		//Find ground planes & apply field to.
		for (TActorIterator<AGroundPlane> Iterator = TActorIterator<AGroundPlane>(GetWorld()); Iterator; ++Iterator)
		{
			if (IsValid(*Iterator))
			{
				if (Iterator->ApplyField(FieldType, NewlyEffectedLocations))
				{
					//Save effected planes
					EffectedGroundPlanes.Add(*Iterator);
				}
			}
		}
	}
	else
	{
		//Apply field to planes
		for (AGroundPlane* EachGroundPlane : EffectedGroundPlanes)
		{
			if (IsValid(EachGroundPlane))
			{
				EachGroundPlane->ApplyField(FieldType, NewlyEffectedLocations);
			}
		}
	}

	//Affect tiles
	TSet<ATile*> NewlyEffectedTiles;
	UGridLibrary::OverlapShape(GetWorld(), Locations, NewlyEffectedTiles, TArray<AActor*>());
	for (ATile* EachNewlyEffectedTile : NewlyEffectedTiles.Difference(EffectedTiles))
	{
		EachNewlyEffectedTile->ApplyField(FieldType);
		EffectedTiles.Add(EachNewlyEffectedTile);
	}

	Super::Affect(Locations);
}

/*
 * Undoes this effect.
 *
 * @param Locations - The locations undo the effect in.
 */
void UApplyField::Unaffect(const TSet<FIntPoint>& Locations)
{
	//Remove invalid planes
	for (TSet<AGroundPlane*>::TIterator Itterator = EffectedGroundPlanes.CreateIterator(); Itterator; ++Itterator)
	{
		if (!IsValid(*Itterator))
		{
			Itterator.RemoveCurrent();
		}
	}

	//Remove field from planes
	for (AGroundPlane* EachGroundPlane : EffectedGroundPlanes)
	{
		if (IsValid(EachGroundPlane))
		{
			EachGroundPlane->RemoveField(FieldType, Locations);
		}
	}

	//Remove from tiles
	TSet<FIntPoint> NewEffectedLocations = EffectedLocations.Difference(Locations);
	for (TSet<ATile*>::TIterator Iterator = EffectedTiles.CreateIterator(); Iterator; ++Iterator)
	{
		if (IsValid(*Iterator) && (*Iterator)->GetSubTileLocations().Intersect(NewEffectedLocations).IsEmpty())
		{
			(*Iterator)->RemoveField(FieldType);
			Iterator.RemoveCurrent();
		}
	}
	EffectedLocations = NewEffectedLocations;
}
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */