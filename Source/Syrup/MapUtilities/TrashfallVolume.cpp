// Fill out your copyright notice in the Description page of Project Settings.

#include "TrashfallVolume.h"

#include "Kismet/GameplayStatics.h"
#include "Syrup/Systems/SyrupGameMode.h"
#include "Syrup/Tiles/GridLibrary.h"
#include "Syrup/Tiles/Trash.h"
#include "Components/BoxComponent.h"
#include "Syrup/Tiles/Trash.h"

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
 * Causes this volume to claim the given trash.
 *
 * @param Trash - The trash to claim.
 */
UFUNCTION()
void ATrashfallVolume::ClaimTrash(ATrash* Trash)
{
	Trash->OnDestroyed.AddDynamic(this, &ATrashfallVolume::ReciveTrashDestoryed);
	Trash->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	NumTrash++;
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
	RNG = FRandomStream(SpawnSeed);
	NumTiles = FMath::Max(1, SpawnArea->GetScaledBoxExtent().SizeSquared2D() / 1558.845726811990);
	BadLocations = TSet<FIntPoint>();

	TArray<AActor*> AttachedTrashPieces;
	GetAttachedActors(AttachedTrashPieces);

	if ((!AttachedTrashPieces.IsEmpty() && AttachedTrashPieces[0]->GetClass() != TrashType.Get()))
	{
		for (AActor* EachAttachedTrashPiece : AttachedTrashPieces)
		{
			EachAttachedTrashPiece->Destroy();
		}

		AttachedTrashPieces.Empty();
	}

	while (AttachedTrashPieces.Num() > NumToMaintain * InitalTrashPercent)
	{
		AttachedTrashPieces.Last()->Destroy();
		AttachedTrashPieces.RemoveAt(AttachedTrashPieces.Num() - 1);
	}

	if (IsValid(TrashType))
	{
		while (NumTrash < NumToMaintain * InitalTrashPercent)
		{
			if (!SpawnTrashInBox())
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
 * @param Triggerer - The tile that triggered this effect.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect.
 */
void ATrashfallVolume::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const ATile* Triggerer, const TSet<FIntPoint>& LocationsToTrigger)
{
	BadLocations = TSet<FIntPoint>();

	if (TriggerType == ETileEffectTriggerType::TrashSpawn && NumTrash < NumToMaintain)
	{
		int TrashToSpawn = 0;
		if (TurnsBetweenSpawns)
		{
			int TrashPerTurn = 1 / TurnsBetweenSpawns;
			int YesterdayDayNumber = Cast<ASyrupGameMode>(UGameplayStatics::GetGameMode(this))->DayNumber - 1;
			TrashToSpawn = FMath::Min(TrashPerTurn + FMath::Frac(TrashPerTurn * YesterdayDayNumber), NumToMaintain - NumTrash);
		}
		else
		{
			TrashToSpawn = NumToMaintain - NumTrash;
		}
		while (TrashToSpawn >= 1)
		{
			TrashToSpawn--;
			SpawnTrashInBox();
		}
	}
}

/**
 * Spawns a single trash inside the spawn area.
 * 
 * @param bAttachTrash - Whether or not to attach the trash to this.
 */
bool ATrashfallVolume::SpawnTrashInBox()
{
	TSet<FIntPoint> RelativeTileLocations = TrashType.GetDefaultObject()->GetRelativeSubTileLocations();
	int Count = 0;
	while (Count++ < 50)
	{
		//Gets a random transform in spawn area and converts it to a grid transform.
		FTransform SpawnWorldTransform = FTransform(FQuat(FVector::UpVector, RNG.FRandRange(0.f, TWO_PI)), GetActorTransform().TransformPosition(RNG.RandPointInBox(FBox(-SpawnArea->GetUnscaledBoxExtent(), SpawnArea->GetUnscaledBoxExtent()))));

		FGridTransform SpawnTransform = UGridLibrary::WorldTransformToGridTransform(SpawnWorldTransform);

		if (BadLocations.Contains(SpawnTransform.Location))
		{
			continue;
		}

		TSet<FIntPoint> SpawnLocations = UGridLibrary::TransformShape(RelativeTileLocations, SpawnTransform);
		if (SpawnLocations.Difference(BadLocations).Num() != SpawnLocations.Num())
		{
			continue;
		}

		for (FIntPoint EachSpawnLocation : SpawnLocations)
		{
			FVector WorldLocation = UGridLibrary::GridLocationToWorldLocation(EachSpawnLocation);
			if (GetWorld()->LineTraceTestByChannel(WorldLocation + FVector(0, 0, 1), WorldLocation + FVector(0, 0, -0.05), ECollisionChannel::ECC_GameTraceChannel2))
			{
				BadLocations.Add(EachSpawnLocation);
				goto endOfLoop;
			}
		}

		ATrash* SpawnedTrash = SpawnTrash(SpawnWorldTransform);
		ClaimTrash(SpawnedTrash);		
		return true;
		endOfLoop:;
	}

	return false;
}
