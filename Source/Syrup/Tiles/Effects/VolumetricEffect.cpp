// Fill out your copyright notice in the Description page of Project Settings.


#include "VolumetricEffect.h"

#include "Syrup/Tiles/Tile.h"
#include "VolumetricEffectActor.h"


/* \/ ================= \/ *\
|  \/ UVolumetricEffect \/  |
\* \/ ================= \/ */
/**
 * Causes this effect.
 *
 * @param EffectedLocations - The locations to effect.
 * @param EffectedTiles - The tiles to effect.
 * @param EffectedNonTileLocations - The locations that are not covered by tiles to effect.
 * @param AffecterTile - The tile doing the affecting.
 */
void UVolumetricEffect::Affect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile)
{
	if (!IsValid(VolumeActor))
	{
		FActorSpawnParameters SpawnParams = FActorSpawnParameters();
		SpawnParams.Owner = AffecterTile;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Name = FName(AffecterTile->GetName() + " " + GetClass()->GetName() + " Actor");

		VolumeActor = AffecterTile->GetWorld()->SpawnActor<AVolumetricEffectActor>(SpawnParams);
		VolumeActor->SetActorLabel(SpawnParams.Name.ToString());
		VolumeActor->SetOverlapedChannels(GetOverlappedChannels());
		VolumeActor->OnActorBeginOverlap.AddDynamic(this, &UVolumetricEffect::OnBeginOverlap);
		VolumeActor->OnActorEndOverlap.AddDynamic(this, &UVolumetricEffect::OnEndOverlap);
	}

	if (IsValid(VolumeActor))
	{
		VolumeActor->AddTiles(EffectedLocations);
	}
}

/**
 * Undoes the effect of this.
 *
 * @param EffectedLocations - The locations that were effected.
 * @param EffectedTiles - The tiles that were effected.
 * @param EffectedNonTileLocations - The locations that are not covered by tiles to that were effected.
 * @param AffecterTile - The tile doing the affecting.
 */
void UVolumetricEffect::Unaffect(TSet<FIntPoint> EffectedLocations, TSet<ATile*> EffectedTiles, TSet<FIntPoint> EffectedNonTileLocations, ATile* AffecterTile)
{
	if (IsValid(VolumeActor))
	{
		VolumeActor->RemoveTiles(EffectedLocations);
	}
}
/* /\ ================= /\ *\
|  /\ UVolumetricEffect /\  |
\* /\ ================= /\ */