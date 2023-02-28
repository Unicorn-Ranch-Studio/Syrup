// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceSaveData.h"
#include "TileSaveData.h"
#include "SinkSaveData.h"
#include "DamageTakenSaveData.h"
#include "TrashfallSaveData.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SyrupSaveGame.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSaveGame, Log, All);

/**
 * 
 */
UCLASS()
class SYRUP_API USyrupSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	USyrupSaveGame();

	/**
	 * Saves the entire world state.
	 * 
	 * @param WorldContext - An object in the world to save.
	 * @param SlotName - The name of the save slot to put the world in.
	 * @param DynamicTileClasses - The classes of tile that will be spawned or destroyed during runtime.
	 */
	UFUNCTION(BlueprintCallable, Category = "Saving", Meta = (WorldContext = "WorldContext"))
	static void SaveGame(const UObject* WorldContext, const FString& SlotName);

	/**
	 * Loads the entire world.
	 *
	 * @param WorldContext - An object in the world to save.
	 * @param SlotName - The name of the save slot to load the world from.
	 * @param DynamicTileClasses - The classes of tile that will be spawned or destroyed during runtime.
	 */
	UFUNCTION(BlueprintCallable, Category = "Saving", Meta = (WorldContext = "WorldContext"))
	static void LoadGame(const UObject* WorldContext, const FString& SlotName);

private:

	/* -------------------- *\
	\* \/ Saving Helpers \/ */

	/**
	 * Stores a tile's class & transform.
	 *
	 * @param Tile - The tile whose data to store.
	 */
	void StoreTileData(const ATile* Tile);

	/**
	 * Stores a tile's resources.
	 *
	 * @param Tile - The tile whose produced resources should be saved.
	 */
	void StoreTileResourceData(const ATile* Tile);

	/**
	 * Stores a tile's sinks.
	 *
	 * @param Tile - The tile whose sinks should be saved.
	 */
	void StoreTileSinkData(const ATile* Tile);

	/* /\ Saving Helpers /\ *\
	\* -------------------- */

	/* --------------------- *\
	\* \/ Loading Helpers \/ */

	/**
	 * Destroys all tiles that could have been spawned during runtime.
	 */
	void DestoryDynamicTiles() const;

	/**
	 * Spawns the tiles from the data stored.
	 */
	void SpawnTiles();

	/**
	 * Sets the sink amounts from the data stored.
	 */
	void UpdateSinkAmounts() const;

	/**
	 * Sets the damage taken from the data stored.
	 */
	void UpdateDamageTaken() const;

	/**
	 * Allocates all resources from the data stored.
	 */
	void AllocateResources() const;

	/**
	 * Sets the trashfall links from the data stored.
	 */
	void UpdateTrashfallLinks() const;

	/**
	 * Gets the tile at a given location.
	 */
	ATile* GetTileAtLocation(const FIntPoint& LocationToSearch) const;

	/* /\ Loading Helpers /\ *\
	\* --------------------- */

	//Stores the type and position of each dynamic tile.
	UPROPERTY()
	TArray<FTileSaveData> TileData = TArray<FTileSaveData>();
	
	//Stores resources and what they link.
	UPROPERTY()
	TArray<FResourceSaveData> ResourceData = TArray<FResourceSaveData>();
	
	//Stores the amount stored in each sink.
	UPROPERTY()
	TArray<FSinkSaveData> SinkData = TArray<FSinkSaveData>();
	
	//Stores the amount stored in each sink.
	UPROPERTY()
	TArray<FDamageTakenSaveData> DamageTakenData = TArray<FDamageTakenSaveData>();
	
	//Stores the amount stored in each sink.
	UPROPERTY()
	TArray<FTrashfallSaveData> TrashfallData = TArray<FTrashfallSaveData>();

	//Stores the players location.
	UPROPERTY()
	FVector PlayerLocation;

	//Number of days that have passed +1.
	UPROPERTY()
	int DayNumber = 1;

	//All of the potentially relevant for data loading tile locations.
	TMap<FIntPoint, ATile*> LocationsToTiles = TMap<FIntPoint, ATile*>();

	//The classes to save the tile data for.
	TArray<TSubclassOf<ATile>> DynamicTileClasses;

	//The world being saved/loaded.
	UWorld* World = nullptr;
};
