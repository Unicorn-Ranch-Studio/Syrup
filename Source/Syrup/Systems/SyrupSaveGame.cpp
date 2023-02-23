// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Syrup/Tiles/Plant.h"
#include "Syrup/Tiles/Trash.h"
#include "Syrup/Tiles/Resources/ResourceFaucet.h"
#include "Syrup/Tiles/Resources/Resource.h"

/**
 * Saves the entire world state.
 * 
 * @param WorldContext - An object in the world to save.
 * @param SlotName - The name of the save slot to put the world in.
 * @param DynamicTileClasses - The classes of tile that will be spawned or destroyed during runtime.
 */
void USyrupSaveGame::SaveGame(const UObject* WorldContext, const FString& SlotName, TArray<TSubclassOf<ATile>> DynamicTileClasses)
{
	if (!IsValid(WorldContext) || !IsValid(WorldContext->GetWorld()))
	{
		return;
	}

	USyrupSaveGame* Save = NewObject<USyrupSaveGame>();
	UWorld* World = WorldContext->GetWorld();

	for (TActorIterator<ATile> EachTile(World); EachTile; ++EachTile)
	{
		TSubclassOf<ATile> TileClass = EachTile->GetClass();
		if (DynamicTileClasses.Contains(DynamicTileClasses))
		{
			Save->TileData.Add(FTileSaveData(EachTile->GetGridTransform(), TileClass));
		}

		if (IResourceFaucet* EachFaucet = Cast<IResourceFaucet>(*EachTile))
		{
			FFaucetSaveData DataToSave = FFaucetSaveData(EachTile->GetGridTransform().Location);
			for (UResource* EachProducedResource : EachFaucet->GetProducedResources())
			{
				UResourceSink* LinkedSink = EachProducedResource->GetLinkedSink();

				DataToSave.TargetLocation = LinkedSink->GetOwner<ATile>()->GetGridTransform().Location;
				DataToSave.SinkName = LinkedSink->GetFName();

				Save->FaucetData.Add(DataToSave);
			}
		}
	}
}

/**
 * Loads the entire world.
 *
 * @param WorldContext - An object in the world to save.
 * @param SlotName - The name of the save slot to load the world from.
 * @param DynamicTileClasses - The classes of tile that will be spawned or destroyed during runtime.
 */
void USyrupSaveGame::LoadGame(const UObject* WorldContext, const FString& SlotName, TArray<TSubclassOf<ATile>> DynamicTileClasses)
{
	if (!IsValid(WorldContext) || !IsValid(WorldContext->GetWorld()))
	{
		return;
	}

	USyrupSaveGame* Save = Cast<USyrupSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	UWorld* World = WorldContext->GetWorld();

	if (!IsValid(Save))
	{
		return;
	}

	for (TActorIterator<ATile> EachTile(World); EachTile; ++EachTile)
	{
		TSubclassOf<ATile> TileClass = EachTile->GetClass();
		if (DynamicTileClasses.Contains(DynamicTileClasses))
		{
			EachTile->Destroy();
		}
	}

	TMap<FIntPoint, ATile*> LocationsToTiles = TMap<FIntPoint, ATile*>();
	for (FTileSaveData EachTileDataum : Save->TileData)
	{
		FTransform ActorTransfrom = UGridLibrary::GridTransformToWorldTransform(EachTileDataum.TileTransfrom);
		LocationsToTiles.Add(EachTileDataum.TileTransfrom.Location, World->SpawnActor<ATile>(EachTileDataum.TileClass, ActorTransfrom));
	}

	for (FFaucetSaveData EachFaucetDataum : Save->FaucetData)
	{
		EachFaucetDataum.Location
	}
}