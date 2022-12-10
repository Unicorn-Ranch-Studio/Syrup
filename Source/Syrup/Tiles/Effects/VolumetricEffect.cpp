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

		VolumeActor = GetWorld()->SpawnActor<AVolumetricEffectActor>(SpawnParams);
		VolumeActor->SetActorLabel(GetOwner()->GetName() + " " + GetClass()->GetName() + " Actor");
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

/*
 * Undoes this effect.
 *
 * @param Locations - The locations undo the effect in.
 */
void UVolumetricEffect::Unaffect(const TSet<FIntPoint>& Locations)
{
	if (IsValid(VolumeActor))
	{
		VolumeActor->RemoveTiles(Locations);
		Super::Unaffect(Locations);
	}
}

/**
 * Destroys the collision actor.
 *
 * @param	bDestroyingHierarchy  - True if the entire component hierarchy is being torn down, allows avoiding expensive operations
 */
void UVolumetricEffect::OnComponentDestroyed(bool bDestroyingHierarchy) {
	if (IsValid(VolumeActor))
	{
		VolumeActor->Destroy();
	}

	Super::OnComponentDestroyed(bDestroyingHierarchy);
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