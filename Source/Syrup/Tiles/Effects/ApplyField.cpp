// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyField.h"

#include "Syrup/Tiles/Tile.h"
#include "EngineUtils.h"

/* \/ =========== \/ *\
|  \/ UApplyField \/  |
\* \/ =========== \/ */
/*
 * Causes this effect.
 *
 * @param TriggerType - The type of effects that are currently being triggered.
 * @param Locations - The locations to effect.
 */
void UApplyField::Affect(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& Locations)
{
	if (TriggerType == ETileEffectTriggerType::Persistent)
	{
		for (TSet<AGroundPlane*>::TIterator Itterator = EffectedGroundPlanes.CreateIterator(); Itterator; ++Itterator)
		{
			if (!IsValid(*Itterator))
			{
				Itterator.RemoveCurrent();
			}
		}

		TSet<FIntPoint> NewlyEffectedLocations = Locations.Difference(EffectedLocations);

		if (EffectedGroundPlanes.IsEmpty())
		{
			for (TActorIterator<AGroundPlane> Iterator = TActorIterator<AGroundPlane>(GetWorld()); Iterator; ++Iterator)
			{
				if (IsValid(*Iterator))
				{
					if (Iterator->ApplyField(FieldType, NewlyEffectedLocations))
					{
						EffectedGroundPlanes.Add(*Iterator);
						break;
					}
				}
			}
		}
		else
		{
			for (AGroundPlane* EachGroundPlane : EffectedGroundPlanes)
			{
				if (IsValid(EachGroundPlane))
				{
					EachGroundPlane->ApplyField(FieldType, NewlyEffectedLocations);
				}
			}
		}

		TSet<ATile*> NewlyEffectedTiles;
		UGridLibrary::OverlapShape(GetWorld(), NewlyEffectedLocations, NewlyEffectedTiles, TArray<AActor*>());
		for (ATile* EachNewlyEffectedTile : NewlyEffectedTiles)
		{
			EachNewlyEffectedTile->ApplyField(FieldType);
			EffectedTiles.Add(EachNewlyEffectedTile);
		}
		Super::Affect(TriggerType, Locations);
	}
}

/*
 * Undoes this effect.
 *
 * @param TriggerType - The type of effects that are currently being undone.
 * @param Locations - The locations to undo the effect on.
 */
void UApplyField::Unaffect(const ETileEffectTriggerType TriggerType)
{
	if (TriggerType == ETileEffectTriggerType::Persistent)
	{
		for (TSet<AGroundPlane*>::TIterator Itterator = EffectedGroundPlanes.CreateIterator(); Itterator; ++Itterator)
		{
			if (!IsValid(*Itterator))
			{
				Itterator.RemoveCurrent();
			}
		}

		for (AGroundPlane* EachGroundPlane : EffectedGroundPlanes)
		{
			EachGroundPlane->RemoveField(FieldType, EffectedLocations);
		}

		for (ATile* EachEffectedTile : EffectedTiles)
		{
			EachEffectedTile->RemoveField(FieldType);
		}
	}
}
/* /\ =========== /\ *\
|  /\ UTileEffect /\  |
\* /\ =========== /\ */