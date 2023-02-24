// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Syrup/Tiles/Plant.h"
#include "Syrup/Tiles/Trash.h"
#include "Syrup/Tiles/Resources/ResourceFaucet.h"
#include "Syrup/Tiles/Resources/Resource.h"


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
	UWorld* World = WorldContext->GetWorld();

	Save->DestoryDynamicTiles(World);

	TMap<FIntPoint, ATile*> LocationsToTiles = TMap<FIntPoint, ATile*>();
	Save->SpawnTiles(World, LocationsToTiles);
	Save->UpdateSinkAmounts(LocationsToTiles);
	Save->CreateResources(LocationsToTiles);
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
	TSubclassOf<ATile> TileClass = Tile->GetClass();
	if (DynamicTileClasses.Contains(TileClass))
	{
		TileData.Add(FTileSaveData(Tile->GetGridTransform(), TileClass));
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
		SinkData.Add(EachSink->GetAllocationAmount());
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
void USyrupSaveGame::DestoryDynamicTiles(UWorld* World)
{
	for (TActorIterator<ATile> EachTile(World); EachTile; ++EachTile)
	{
		TSubclassOf<ATile> TileClass = EachTile->GetClass();
		if (DynamicTileClasses.Contains(TileClass))
		{
			EachTile->Destroy();
		}
	}
}

void USyrupSaveGame::SpawnTiles(UWorld* World, TMap<FIntPoint, ATile*>& LocationsToTiles)
{
	for (FTileSaveData EachTileDataum : TileData)
	{
		FTransform ActorTranfrom = UGridLibrary::GridTransformToWorldTransform(EachTileDataum.TileTransfrom);
		ATile* NewTile = World->SpawnActor<ATile>(EachTileDataum.TileClass, ActorTranfrom);
	}
}


void USyrupSaveGame::UpdateSinkAmounts(const TMap<FIntPoint, ATile*> LocationsToTiles)
{

}

void USyrupSaveGame::CreateResources(const TMap<FIntPoint, ATile*> LocationsToTiles)
{

}

/* /\ Loading Helpers /\ *\
\* --------------------- */