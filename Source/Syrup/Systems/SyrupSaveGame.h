// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TileSaveData.h"
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
	 */
	UFUNCTION(BlueprintCallable, Category = "Saving", Meta = (WorldContext = "WorldContext"))
	static void SaveGame(const UObject* WorldContext, const FString& SlotName);

	/**
	 * Loads the entire world.
	 *
	 * @param WorldContext - An object in the world to save.
	 * @param SlotName - The name of the save slot to load the world from.
	 * @param EmptyLevel - The empty level to load before spawning in actors. Will not load level if Null.
	 */
	UFUNCTION(BlueprintCallable, Category = "Saving", Meta = (WorldContext = "WorldContext"))
	static void LoadGame(const UObject* WorldContext, const FString& SlotName, const TSoftObjectPtr<UWorld> EmptyLevel);

private:
	/**
	 * Stores an array of tiles and their resource sinks.
	 * 
	 * @param Tiles - The tiles to store
	 */
	UFUNCTION()
	void StoreTiles(TArray<AActor*> Tiles);


	/**
	 * Destroys all the actors.
	 *
	 * @param Actors - The actors to destroy.
	 */
	static void DestroyActors(TArray<AActor*> Actors);

	UPROPERTY()
	TArray<FTileSaveData> TileData = TArray<FTileSaveData>();

};
