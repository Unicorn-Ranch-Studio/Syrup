// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Syrup/Tiles/Plant.h"
#include "Syrup/Tiles/Trash.h"
#include "Syrup/Tiles/Resources/ResourceFaucet.h"
#include "Syrup/Tiles/Resources/Resource.h"

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

	TMap<FIntPoint, ATile*> LocationsToTiles = TMap<FIntPoint, ATile*>();
	Save->SpawnTiles(LocationsToTiles);
	Save->UpdateSinkAmounts(LocationsToTiles);
	Save->UpdateDamageTaken(LocationsToTiles);
	Save->AllocateResources(LocationsToTiles);
}

/* -------------------- *\
\* \/ Saving Helpers \/ */

/**
 * Stores a tile's class & transform.
 *
 * @param Tile - The tile whose data to store.
 */
void USyrupSaveGame::StoreTileData(ATile* Tile)
{
	for (TSubclassOf<ATile> EachDynamicTileClass : DynamicTileClasses)
	{
		if (Tile->IsA(EachDynamicTileClass.Get()))
		{
			TSubclassOf<ATile> TileClass = Tile->GetClass();
			TileData.Add(FTileSaveData(Tile->GetGridTransform(), TileClass));

			if (TileClass == APlant::StaticClass())
			{
				APlant* Plant = Cast<APlant>(Tile);
				DamageTakenData.Add(FDamageTakenSaveData(Tile->GetGridTransform().Location, Plant->GetDamageTaken()));
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
void USyrupSaveGame::StoreTileResourceData(ATile* Tile)
{
	if (IResourceFaucet* EachFaucet = Cast<IResourceFaucet>(Tile))
	{
		FResourceSaveData DataToSave = FResourceSaveData(Tile->GetGridTransform().Location);
		for (UResource* EachProducedResource : EachFaucet->GetProducedResources())
		{
			if (EachProducedResource->IsAllocated())
			{
				UResourceSink* LinkedSink = EachProducedResource->GetLinkedSink();

				DataToSave.FaucetLocation = LinkedSink->GetOwner<ATile>()->GetGridTransform().Location;
				DataToSave.SinkName = LinkedSink->GetFName();

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
void USyrupSaveGame::StoreTileSinkData(ATile* Tile)
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
 *
 * @param World - The world to destroy tiles in.
 */
void USyrupSaveGame::DestoryDynamicTiles()
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
 *
 * @param World - The world to spawn the tiles in.
 * @param LocationsToTiles - Will be set to contain the locations of each tile.
 */
void USyrupSaveGame::SpawnTiles(TMap<FIntPoint, ATile*>& LocationsToTiles)
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
 *
 * @param LocationsToTiles - The locations of every tile containing a sink.
 */
void USyrupSaveGame::UpdateDamageTaken(const TMap<FIntPoint, ATile*> LocationsToTiles)
{
	for (FDamageTakenSaveData EachDamageTakenDatum : DamageTakenData)
	{
		Cast<APlant>(GetTileAtLocation(EachDamageTakenDatum.Location, LocationsToTiles))->SetDamageTaken(EachDamageTakenDatum.Amount);
	}
}

/**
 * Sets the damage taken from the data stored.
 *
 * @param LocationsToTiles - The locations of every damaged plant.
 */
void USyrupSaveGame::UpdateSinkAmounts(const TMap<FIntPoint, ATile*> LocationsToTiles)
{
	for (FSinkSaveData EachSinkDatum : SinkData)
	{
		AActor* Owner = GetTileAtLocation(EachSinkDatum.Location, LocationsToTiles);
		if (!IsValid(Owner))
		{
			UE_LOG(LogSaveGame, Error, TEXT("Sink: %s Owner not found at %s"), *EachSink->GetFName().ToString(), *EachSinkDatum.Location.ToString());
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
		UE_LOG(LogSaveGame, Error, TEXT("Sink: %s not found on %s"), *EachSink->GetFName().ToString(), *Owner->GetName());
	nextLoop:
	}
}

/**
 * Allocates all resources from the data stored.
 *
 * @param LocationsToTiles - The locations of every tile containing a sink or faucet.
 */
void USyrupSaveGame::AllocateResources(const TMap<FIntPoint, ATile*> LocationsToTiles)
{

}


/**
 * Gets the tile at a given location.
 *
 * @param LocationToSearch - The location to find the tile at.
 * @param LocationsToTiles - The locations of tiles, if LocationToSearch is not contained, the world will be queried.
 */
ATile* USyrupSaveGame::GetTileAtLocation(const FIntPoint LocationToSearch, const TMap<FIntPoint, ATile*> LocationsToTiles)
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