// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "Syrup/Tiles/Plant.h"
#include "Syrup/Tiles/SpiritPlant.h"
#include "Syrup/Tiles/Trash.h"
#include "Syrup/Tiles/Resources/ResourceFaucet.h"
#include "Syrup/Tiles/Resources/Resource.h"

/**
 * Saves the entire world state.
 * 
 * @param WorldContext - An object in the world to save.
 * @param SlotName - The name of the save slot to put the world in.
 */
void USyrupSaveGame::SaveGame(const UObject* WorldContext, const FString& SlotName)
{
	if (!IsValid(WorldContext) || !IsValid(WorldContext->GetWorld()))
	{
		return;
	}

	USyrupSaveGame* Save = NewObject<USyrupSaveGame>();
	UWorld* World = WorldContext->GetWorld();

	TArray<AActor*> SpiritPlants;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASpiritPlant::StaticClass(), SpiritPlants);
	Save->StoreTiles(SpiritPlants);

	TArray<AActor*> Plants;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, APlant::StaticClass(), Plants);
	Save->StoreTiles(Plants);

	TArray<AActor*> Trash;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ATrash::StaticClass(), Trash);
	Save->StoreTiles(Trash);

	bool bSuccess = UGameplayStatics::SaveGameToSlot(Save, SlotName, 0);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(bSuccess ? "Saving Successful" : " * Saving FAILED * "))
}

/**
 * Loads the entire world.
 *
 * @param WorldContext - An object in the world to save.
 * @param SlotName - The name of the save slot to load the world from.
 */
void USyrupSaveGame::LoadGame(const UObject* WorldContext, const FString& SlotName, const TSoftObjectPtr<UWorld> EmptyLevel)
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

	TArray<AActor*> ActorsToDestroy;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASpiritPlant::StaticClass(), ActorsToDestroy);
	DestroyActors(ActorsToDestroy);
	UGameplayStatics::GetAllActorsOfClass(WorldContext, APlant::StaticClass(), ActorsToDestroy);
	DestroyActors(ActorsToDestroy);
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ATrash::StaticClass(), ActorsToDestroy);
	DestroyActors(ActorsToDestroy);

	for (FTileSaveData EachTileData : Save->TileData)
	{
		FTransform ActorTranfrom = UGridLibrary::GridTransformToWorldTransform(EachTileData.TileTransfrom);
		ATile* NewTile = World->SpawnActor<ATile>(EachTileData.TileClass, ActorTranfrom);
	}
}


/**
 * Stores an array of tiles and their resource sinks.
 *
 * @param Tiles - The tiles to store
 */
void USyrupSaveGame::StoreTiles(TArray<AActor*> Tiles)
{
	for (AActor* EachTile : Tiles)
	{
		ATile* Tile = Cast<ATile>(EachTile);
		TArray<FSinkTargetSaveData> FaucetLinks = TArray<FSinkTargetSaveData>();
		if (IResourceFaucet* Faucet = Cast<IResourceFaucet>(Tile))
		{
			for (UResource* EachProducedResource : Faucet->GetProducedResources())
			{
				if (EachProducedResource->IsAllocated())
				{
					UResourceSink* LinkedSink = EachProducedResource->GetLinkedSink();
					UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *LinkedSink->GetFName().ToString());
					FaucetLinks.Add(FSinkTargetSaveData(LinkedSink->GetOwner<ATile>()->GetGridTransform().Location, LinkedSink->GetFName()));
				}
			}
		}

		TileData.Add(FTileSaveData(Tile->GetGridTransform(), Tile->GetClass(), FaucetLinks));
	}
}

/**
 * Destroys all the actors.
 *
 * @param Actors - The actors to destroy.
 */
void USyrupSaveGame::DestroyActors(TArray<AActor*> Actors)
{
	for (AActor* EachActor : Actors)
	{
		Actors->Destroy();
	}
}