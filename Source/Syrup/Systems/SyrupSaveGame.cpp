// Fill out your copyright notice in the Description page of Project Settings.


#include "SyrupSaveGame.h"

#include "Syrup/Tiles/Plant.h"
#include "Syrup/Tiles/Trash.h"
#include "Kismet/GameplayStatics.h"

/**
 * Saves the entire world state.
 * 
 * @param WorldContext - An object in the world to save.
 * @param SlotName - The name of the save slot to put the world in.
 */
void USyrupSaveGame::SaveGame(UObject* WorldContext, FString SlotName)
{
	if (!IsValid(WorldContext) || !IsValid(WorldContext->GetWorld()))
	{
		return;
	}

	USyrupSaveGame* Save = NewObject<USyrupSaveGame>();
	UWorld* World = WorldContext->GetWorld();

	TArray<AActor*> Plants;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, APlant::StaticClass(), Plants);
	Save->StoreTiles(Plants);

	TArray<AActor*> Trash;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ATrash::StaticClass(), Trash);
	Save->StoreTiles(Trash);


}

/**
 * Loads the entire world.
 *
 * @param WorldContext - An object in the world to save.
 * @param SlotName - The name of the save slot to load the world from.
 */
void USyrupSaveGame::LoadGame(UObject* WorldContext, FString SlotName)
{

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
	}
}