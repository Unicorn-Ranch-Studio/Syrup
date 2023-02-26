// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Syrup/Tiles/Plant.h"
#include "Syrup/Tiles/Trash.h"
#include "Syrup/Tiles/Resources/ResourceFaucet.h"
#include "Syrup/Tiles/Resources/Resource.h"
#include "Syrup/MapUtilities/TrashfallVolume.h"

DEFINE_LOG_CATEGORY(LogSaveGame);

USyrupSaveGame::USyrupSaveGame()
{
	DynamicTileClasses = TArray<TSubclassOf<ATile>>();
	DynamicTileClasses.Add(APlant::StaticClass());
	DynamicTileClasses.Add(ATrash::StaticClass());
}

/**
 * Saves the entire world state.
 * 
 * @param WorldContext - An object in the world to save.
 * @param SlotName - The name of the save slot to put the world in.
 * @param DynamicTileClasses - The classes of tile that will be spawned or destroyed during runtime.
 */
void USyrupSaveGame::SaveGame(const UObject* WorldContext, const FString& SlotName)
{
	if (!IsValid(WorldContext) || !IsValid(WorldContext->GetWorld()))
	{
		return;
	}

	USyrupSaveGame* Save = NewObject<USyrupSaveGame>();
	UWorld* World = WorldContext->GetWorld();

	for (TActorIterator<ATile> EachTile(World); EachTile; ++EachTile)
	{
		Save->StoreTileData(*EachTile);
		Save->StoreTileSinkData(*EachTile);
		Save->StoreTileResourceData(*EachTile);
	}
	Save->PlayerLocation = UGameplayStatics::GetPlayerPawn(WorldContext, 0)->GetActorLocation();

	UGameplayStatics::SaveGameToSlot(Save, SlotName, 0);
}

/**
 * Loads the entire world.
 *
 * @param WorldContext - An object in the world to save.
 * @param SlotName - The name of the save slot to load the world from.
 * @param DynamicTileClasses - The classes of tile that will be spawned or destroyed during runtime.
 */
void USyrupSaveGame::LoadGame(const UObject* WorldContext, const FString& SlotName)
{
	USyrupSaveGame* Save = Cast<USyrupSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!IsValid(Save) || !IsValid(WorldContext) || !IsValid(WorldContext->GetWorld()))
	{
		return;
	}
	Save->World = WorldContext->GetWorld();

	Save->DestoryDynamicTiles();

	Save->SpawnTiles();
	Save->UpdateSinkAmounts();
	Save->UpdateDamageTaken();
	Save->AllocateResources();
	Save->UpdateTrashfallLinks();
	UGameplayStatics::GetPlayerPawn(WorldContext, 0)->SetActorLocation(Save->PlayerLocation);
}

/* -------------------- *\
\* \/ Saving Helpers \/ */

/**
 * Stores a tile's class & transform.
 *
 * @param Tile - The tile whose data to store.
 */
void USyrupSaveGame::StoreTileData(const ATile* Tile)
{
	for (TSubclassOf<ATile> EachDynamicTileClass : DynamicTileClasses)
	{
		if (Tile->IsA(EachDynamicTileClass.Get()))
		{
			TSubclassOf<ATile> TileClass = Tile->GetClass();
			TileData.Add(FTileSaveData(Tile->GetGridTransform(), TileClass));

			if (Tile->IsA(APlant::StaticClass()))
			{
				const APlant* Plant = Cast<APlant>(Tile);
				DamageTakenData.Add(FDamageTakenSaveData(Tile->GetGridTransform().Location, Plant->GetDamageTaken()));
			}
			else if (Tile->IsA(ATrash::StaticClass()))
			{
				ATrashfallVolume* ParentVolume = Cast<ATrashfallVolume>(Tile->GetRootComponent()->GetAttachParentActor());
				if (IsValid(ParentVolume))
				{
					TrashfallData.Add(FTrashfallSaveData(Tile->GetGridTransform().Location, ParentVolume));
				}
			}
			return;
		}
	}
}

/**
 * Stores a tile's resources.
 * 
 * @param Tile - The tile whose produced resources should be saved.
 */
void USyrupSaveGame::StoreTileResourceData(const ATile* Tile)
{
	if (const IResourceFaucet* EachFaucet = Cast<IResourceFaucet>(Tile))
	{
		FResourceSaveData DataToSave = FResourceSaveData(Tile->GetGridTransform().Location);
		for (UResource* EachProducedResource : EachFaucet->GetProducedResources())
		{
			if (EachProducedResource->IsAllocated())
			{
				const UResourceSink* LinkedSink = EachProducedResource->GetLinkedSink();
				TScriptInterface<IResourceFaucet> LinkedFaucet;
				EachProducedResource->GetLinkedFaucet(LinkedFaucet);

				DataToSave.FaucetLocation = Cast<ATile>(LinkedFaucet.GetObject())->GetGridTransform().Location;
				DataToSave.SinkLocation = LinkedSink->GetOwner<ATile>()->GetGridTransform().Location;
				DataToSave.SinkName = LinkedSink->GetFName();
				DataToSave.Type = EachProducedResource->GetType();

				ResourceData.Add(DataToSave);
			}
		}
	}
}

/**
 * Stores a tile's sinks.
 *
 * @param Tile - The tile whose sinks should be saved.
 */
void USyrupSaveGame::StoreTileSinkData(const ATile* Tile)
{
	TArray<UResourceSink*> Sinks;
	Tile->GetComponents<UResourceSink>(Sinks);
	for (UResourceSink* EachSink : Sinks)
	{
		SinkData.Add(FSinkSaveData(Tile->GetGridTransform().Location, EachSink->GetFName(), EachSink->GetAllocationAmount()));
	}
}

/* /\ Saving Helpers /\ *\
\* -------------------- */



/* --------------------- *\
\* \/ Loading Helpers \/ */

/**
 * Destroys all tiles that could have been spawned during runtime.
 */
void USyrupSaveGame::DestoryDynamicTiles() const
{
	for (TActorIterator<ATile> EachTile(World); EachTile; ++EachTile)
	{
		TSubclassOf<ATile> TileClass = EachTile->GetClass();
		for (TSubclassOf<ATile> EachDynamicTileClass : DynamicTileClasses)
		{
			if (EachTile->IsA(EachDynamicTileClass.Get()))
			{
				EachTile->Destroy();
				break;
			}
		}
	}
}

/**
 * Spawns the tiles from the data stored.
 */
void USyrupSaveGame::SpawnTiles()
{
	for (FTileSaveData EachTileDatum : TileData)
	{
		FTransform ActorTranfrom = UGridLibrary::GridTransformToWorldTransform(EachTileDatum.TileTransfrom);
		ATile* NewTile = World->SpawnActor<ATile>(EachTileDatum.TileClass, ActorTranfrom);

		LocationsToTiles.Add(EachTileDatum.TileTransfrom.Location, NewTile);
	}
}

/**
 * Sets the sink amounts from the data stored.
 */
void USyrupSaveGame::UpdateDamageTaken() const
{
	for (FDamageTakenSaveData EachDamageTakenDatum : DamageTakenData)
	{
		Cast<APlant>(GetTileAtLocation(EachDamageTakenDatum.Location))->SetDamageTaken(EachDamageTakenDatum.Amount);
	}
}

/**
 * Sets the damage taken from the data stored.
 */
void USyrupSaveGame::UpdateSinkAmounts() const
{
	for (FSinkSaveData EachSinkDatum : SinkData)
	{
		AActor* Owner = GetTileAtLocation(EachSinkDatum.Location);
		if (!IsValid(Owner))
		{
			UE_LOG(LogSaveGame, Error, TEXT("Sink: %s Owner not found at %s"), *EachSinkDatum.Name.ToString(), *EachSinkDatum.Location.ToString());
		}

		TArray<UResourceSink*> Sinks;
		Owner->GetComponents<UResourceSink>(Sinks);

		for (UResourceSink* EachSink : Sinks)
		{
			if (EachSink->GetFName() == EachSinkDatum.Name)
			{
				EachSink->SetAllocationAmount(EachSinkDatum.StoredAmount);
				goto nextLoop;
			}
		}
		UE_LOG(LogSaveGame, Error, TEXT("Sink: %s not found on %s"), *EachSinkDatum.Name.ToString(), *Owner->GetName());
	nextLoop:
		;
	}
}

/**
 * Allocates all resources from the data stored.
 */
void USyrupSaveGame::AllocateResources() const
{
	for (FResourceSaveData ResourceDatum : ResourceData)
	{
		IResourceFaucet* Faucet = Cast<IResourceFaucet>(GetTileAtLocation(ResourceDatum.FaucetLocation));
		if (!Faucet)
		{
			UE_LOG(LogSaveGame, Error, TEXT("Allocating resource from %s to the %s at %s failed to find faucet."), *ResourceDatum.FaucetLocation.ToString(), *ResourceDatum.SinkName.ToString(), *ResourceDatum.SinkLocation.ToString());
			continue;
		}

		ATile* SinkOwner = GetTileAtLocation(ResourceDatum.SinkLocation);
		if (!IsValid(SinkOwner))
		{
			UE_LOG(LogSaveGame, Error, TEXT("Allocating resource from %s to the %s at %s failed to find sink owner."), *ResourceDatum.FaucetLocation.ToString(), *ResourceDatum.SinkName.ToString(), *ResourceDatum.SinkLocation.ToString());
			continue;
		}

		UResourceSink* Sink = nullptr;
		TArray<UResourceSink*> Sinks;
		SinkOwner->GetComponents<UResourceSink>(Sinks);
		for (UResourceSink* EachSink : Sinks)
		{
			if (EachSink->GetFName() == ResourceDatum.SinkName)
			{
				Sink = EachSink;
				break;
			}
		}
		if (!IsValid(Sink))
		{
			UE_LOG(LogSaveGame, Error, TEXT("Allocating resource from %s to the %s at %s failed to find sink."), *ResourceDatum.FaucetLocation.ToString(), *ResourceDatum.SinkName.ToString(), *ResourceDatum.SinkLocation.ToString());
			continue;
		}

		UResource* ResourceToAllocate = nullptr;
		for (UResource* EachProducedResource : Faucet->GetProducedResources())
		{
			if (!EachProducedResource->IsAllocated() && EachProducedResource->GetType() == ResourceDatum.Type)
			{
				ResourceToAllocate = EachProducedResource;
				break;
			}
		}
		if (!IsValid(ResourceToAllocate))
		{
			ResourceToAllocate = Faucet->ProduceResource(ResourceDatum.Type);
		}

		Sink->AllocateResource(ResourceToAllocate, true);
	}
}


/**
 * Sets the trashfall links from the data stored.
 */
void USyrupSaveGame::UpdateTrashfallLinks() const
{
	for (FTrashfallSaveData EachTrashfallDatum : TrashfallData)
	{
		EachTrashfallDatum.Volume->ClaimTrash(Cast<ATrash>(GetTileAtLocation(EachTrashfallDatum.TrashLocation)));
	}
}

/**
 * Gets the tile at a given location.
 */
ATile* USyrupSaveGame::GetTileAtLocation(const FIntPoint& LocationToSearch) const
{
	if (LocationsToTiles.Contains(LocationToSearch))
	{
		return LocationsToTiles.FindRef(LocationToSearch);
	}

	ATile* ReturnValue;
	UGridLibrary::OverlapGridLocation(World, LocationToSearch, ReturnValue, TArray<AActor*>());
	return ReturnValue;
}

/* /\ Loading Helpers /\ *\
\* --------------------- */