// Fill out your copyright notice in the Description page of Project Settings.

#include "Syrup/Systems/SyrupGameMode.h"
#include "Syrup/Tiles/GridLibrary.h"
#include "Syrup/Tiles/Trash.h"
#include "Components/BoxComponent.h"
#include "Syrup/Tiles/Trash.h"

#include "TrashfallVolume.h"

/* \/ ================ \/ *\
|  \/ ATrashfallVolume \/  |
\* \/ ================ \/ */

/**
 * Initializes box component.
 */
ATrashfallVolume::ATrashfallVolume()
{
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(FName("Box Component"));
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = SpawnArea;
}
/**
 * Respawns all trash spawned by this
 */
void ATrashfallVolume::ResetTrashLocations()
{
	Destroyed();
	NumTrash = 0;
	OnConstruction(GetActorTransform());
}

/**
 * Links this to the game mode for receiving trigger events.
 */
void ATrashfallVolume::BeginPlay()
{
	if (!IsValid(TrashType))
	{
		UE_LOG(LogLevel, Warning, TEXT("%s trash type not valid"), *GetName())
		Destroy();
		return;
	}

	Super::BeginPlay();

	TurnsUntilSpawn = TurnsBetweenSpawns;

	ASyrupGameMode::GetTileEffectTriggerDelegate(this).AddDynamic(this, &ATrashfallVolume::ReceiveEffectTrigger);
}

/**
 * Destroys trash spawned by this.
 */
void ATrashfallVolume::Destroyed()
{
	TArray<AActor*> AttachedTrashPieces;
	GetAttachedActors(AttachedTrashPieces);

	for (AActor* EachAttachedTrashPiece : AttachedTrashPieces)
	{
		EachAttachedTrashPiece->Destroy();
	}
}

/**
 * Spawns the appropriate number of trash.
 *
 * @param Transform - The new transform of the volume.
 */
void ATrashfallVolume::OnConstruction(const FTransform& Transform)
{
	NumTiles = FMath::Max(1, SpawnArea->GetScaledBoxExtent().SizeSquared2D() / 1558.845726811990);
	BadLocations = TSet<FIntPoint>();

	TArray<AActor*> AttachedTrashPieces;
	GetAttachedActors(AttachedTrashPieces);

	if (!AttachedTrashPieces.IsEmpty() && AttachedTrashPieces[0]->GetClass() != TrashType.Get())
	{
		for (AActor* EachAttachedTrashPiece : AttachedTrashPieces)
		{
			EachAttachedTrashPiece->Destroy();
		}

		AttachedTrashPieces.Empty();
	}

	while (AttachedTrashPieces.Num() > NumToMaintain)
	{
		AttachedTrashPieces.Last()->Destroy();
		AttachedTrashPieces.RemoveAt(AttachedTrashPieces.Num() - 1);
	}

	if (IsValid(TrashType))
	{
		while (NumTrash < NumToMaintain)
		{
			if (!SpawnTrash(true))
			{
				break;
			}
		}
	}
}

/**
 * Activates the trashfall if trigger type is trash spreads.
 *
 * @param TriggerType - The of trigger that was activated.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect.
 */
void ATrashfallVolume::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& LocationsToTrigger)
{
	BadLocations = TSet<FIntPoint>();

	if (TriggerType == ETileEffectTriggerType::TrashSpread && NumTrash < NumToMaintain && TurnsUntilSpawn-- <= 0)
	{
		SpawnTrash();
	}
}

/**
 * Spawns a single trash inside the spawn area.
 * 
 * @param bAttachTrash - Whether or not to attach the trash to this.
 */
bool ATrashfallVolume::SpawnTrash(bool bAttachTrash)
{
	while (BadLocations.Num() < NumTiles)
	{
		//Gets a random transform in spawn area and converts it to a grid transform.
		FTransform SpawnWorldTransform = FTransform(FQuat(FVector::UpVector, FMath::FRandRange(0.f, /*TWO_PI*/ 0.f)), GetActorTransform().TransformPosition(FMath::RandPointInBox(FBox(-SpawnArea->GetUnscaledBoxExtent(), SpawnArea->GetUnscaledBoxExtent()))));

		FGridTransform SpawnTransform = UGridLibrary::WorldTransformToGridTransform(SpawnWorldTransform).Location;

		if (BadLocations.Contains(SpawnTransform.Location))
		{
			continue;
		}

		TSet<FIntPoint> SpawnLocations = UGridLibrary::TransformShape(TrashType.GetDefaultObject()->GetShape(), SpawnTransform);
		if (SpawnLocations.Difference(BadLocations).Num() != SpawnLocations.Num())
		{
			continue;
		}

		for (FIntPoint EachSpawnLocation : SpawnLocations)
		{
			FVector WorldLocation = UGridLibrary::GridTransformToWorldTransform(FGridTransform(EachSpawnLocation)).GetLocation();
			if (GetWorld()->LineTraceTestByChannel(WorldLocation, WorldLocation + FVector(0, 0, -0.1), ECollisionChannel::ECC_GameTraceChannel2))
			{
				DrawDebugPoint(GetWorld(), WorldLocation, 10, FColor::Red, false, 10);
				BadLocations.Add(EachSpawnLocation);
				goto endOfLoop;
			}
			DrawDebugPoint(GetWorld(), WorldLocation, 10, FColor::Green, false, 10);
		}

		ATrash* SpawnedTrash = GetWorld()->SpawnActor<ATrash>(TrashType, SpawnWorldTransform);
		SpawnedTrash->OnDestroyed.AddDynamic(this, &ATrashfallVolume::ReciveTrashDestoryed);
		NumTrash++;
		TurnsUntilSpawn = TurnsBetweenSpawns;

		//BadLocations = BadLocations.Union(SpawnLocations);

		if (bAttachTrash)
		{
			SpawnedTrash->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		return true;
		endOfLoop:;
	}

	return false;
}
