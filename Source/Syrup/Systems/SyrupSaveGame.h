// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FaucetSaveData.h"
#include "TileSaveData.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SyrupSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SYRUP_API USyrupSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	/**
	 * Saves the entire world state.
	 * 
	 * @param WorldContext - An object in the world to save.
	 * @param SlotName - The name of the save slot to put the world in.
	 * @param DynamicTileClasses - The classes of tile that will be spawned or destroyed during runtime.
	 */
	UFUNCTION(BlueprintCallable, Category = "Saving", Meta = (WorldContext = "WorldContext"))
	static void SaveGame(const UObject* WorldContext, const FString& SlotName, TArray<TSubclassOf<ATile>> DynamicTileClasses);

	/**
	 * Loads the entire world.
	 *
	 * @param WorldContext - An object in the world to save.
	 * @param SlotName - The name of the save slot to load the world from.
	 * @param DynamicTileClasses - The classes of tile that will be spawned or destroyed during runtime.
	 */
	UFUNCTION(BlueprintCallable, Category = "Saving", Meta = (WorldContext = "WorldContext"))
	static void LoadGame(const UObject* WorldContext, const FString& SlotName, TArray<TSubclassOf<ATile>> DynamicTileClasses);

private:

	UPROPERTY()
	TArray<FTileSaveData> TileData = TArray<FTileSaveData>();
	
	UPROPERTY()
	TArray<FFaucetSaveData> FaucetData = TArray<FFaucetSaveData>();
};
