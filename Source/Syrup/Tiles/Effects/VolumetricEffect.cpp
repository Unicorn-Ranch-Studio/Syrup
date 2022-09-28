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
void UVolumetricEffect::Affect(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& Locations)
{
	if (!IsValid(VolumeActor))
	{
		FActorSpawnParameters SpawnParams = FActorSpawnParameters();
		SpawnParams.Owner = GetOwner();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Name = FName(GetOwner()->GetName() + " " + GetClass()->GetName() + " Actor");

		VolumeActor = GetWorld()->SpawnActor<AVolumetricEffectActor>(SpawnParams);
		VolumeActor->SetActorLabel(SpawnParams.Name.ToString());
		VolumeActor->SetCollisionResponses(GetOverlappedChannels(), GetBlockedChannels());
		VolumeActor->OnActorBeginOverlap.AddDynamic(this, &UVolumetricEffect::OnBeginOverlap);
		VolumeActor->OnActorEndOverlap.AddDynamic(this, &UVolumetricEffect::OnEndOverlap);
	}

	if (IsValid(VolumeActor))
	{
		VolumeActor->AddTiles(Locations.Difference(EffectedLocations));
		Super::Affect(TriggerType, Locations);
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
void UVolumetricEffect::Unaffect(const ETileEffectTriggerType TriggerType)
{
	if (IsValid(VolumeActor))
	{
		VolumeActor->Destroy();
	}
}
/* /\ ================= /\ *\
|  /\ UVolumetricEffect /\  |
\* /\ ================= /\ */