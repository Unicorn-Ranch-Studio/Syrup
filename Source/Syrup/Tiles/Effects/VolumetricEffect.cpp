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
		VolumeActor->OnActorBeginOverlap.AddDynamic(this, &UVolumetricEffect::ReceiveBeginOverlap);
		VolumeActor->OnActorEndOverlap.AddDynamic(this, &UVolumetricEffect::ReceiveEndOverlap);
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

/**
 * Applies the effect of this volume when overlap is begun.
 *
 * @param OverlappedActor - The volume that was overlapped.
 * @param OtherActor - The actor that entered the volume.
 */
void UVolumetricEffect::ReceiveBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{	
	if (!OverlappedActors.Contains(OtherActor))
	{
		OverlappedActors.Add(OtherActor);
		OnBeginOverlap(OtherActor);
	}
}

/**
 * Applies the effect of this volume when overlap is ended.
 *
 * @param OverlappedActor - The volume that was left.
 * @param OtherActor - The actor that left the volume.
 */
void UVolumetricEffect::ReceiveEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TSet<AActor*> OverlapingActors;
	OtherActor->GetOverlappingActors(OverlapingActors);
	if (!OverlapingActors.Contains(OverlappedActor))
	{
		OverlappedActors.Remove(OtherActor);
		OnEndOverlap(OtherActor);
	}
}

/* /\ ================= /\ *\
|  /\ UVolumetricEffect /\  |
\* /\ ================= /\ */