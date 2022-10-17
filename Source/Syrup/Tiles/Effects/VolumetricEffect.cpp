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
 * @param Locations - The locations to effect.
 */
void UVolumetricEffect::Affect(const TSet<FIntPoint>& Locations)
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
		Super::Affect(Locations);
	}
}

/**
 * Undoes the effect of this.
 */
void UVolumetricEffect::Unaffect()
{
	if (IsValid(VolumeActor))
	{
		VolumeActor->Destroy();
	}
}
/* /\ ================= /\ *\
|  /\ UVolumetricEffect /\  |
\* /\ ================= /\ */