// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ResourceSaveData.h"
#include "TileSaveData.h"
#include "SinkSaveData.h"
#include "DamageTakenSaveData.h"

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
	 * 
	 * @param World - The world to destroy tiles in.
	 */
	void DestoryDynamicTiles() const;

	/**
	 * Spawns the tiles from the data stored.
	 * 
	 * @param World - The world to spawn the tiles in.
	 * @param LocationsToTiles - Will be set to contain the locations of each tile.
	 */
	void SpawnTiles(TMap<FIntPoint, ATile*>& LocationsToTiles) const;

	/**
	 * Sets the sink amounts from the data stored.
	 *
	 * @param LocationsToTiles - The locations of every tile containing a sink.
	 */
	void UpdateSinkAmounts(const TMap<FIntPoint, ATile*>& LocationsToTiles = TMap<FIntPoint, ATile*>()) const;

	/**
	 * Sets the damage taken from the data stored.
	 *
	 * @param LocationsToTiles - The locations of every damaged plant.
	 */
	void UpdateDamageTaken(const TMap<FIntPoint, ATile*>& LocationsToTiles = TMap<FIntPoint, ATile*>()) const;

	/**
	 * Sets the trashfall links from the data stored.
	 *
	 * @param LocationsToTiles - The locations of every trash spawned by a trashfall.
	 */
	//void UpdateTrashfallLinks(const TMap<FIntPoint, ATile*> LocationsToTiles = TMap<FIntPoint, ATile*>()) const;

	/**
	 * Allocates all resources from the data stored.
	 *
	 * @param LocationsToTiles - The locations of every tile containing a sink or faucet.
	 */
	void AllocateResources(const TMap<FIntPoint, ATile*>& LocationsToTiles = TMap<FIntPoint, ATile*>()) const;

	/**
	 * Gets the tile at a given location.
	 * 
	 * @param WorldContext - An object in the world to query as a fallback.
	 * @param LocationToSearch - The location to find the tile at.
	 * @param LocationsToTiles - The locations of tiles, if LocationToSearch is not contained, the world will be queried.
	 */
	ATile* GetTileAtLocation(const FIntPoint& LocationToSearch, const TMap<FIntPoint, ATile*>& LocationsToTiles = TMap<FIntPoint, ATile*>()) const;
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

	//The classes to save the tile data for.
	TArray<TSubclassOf<ATile>> DynamicTileClasses;

	//The world being saved/loaded.
	UWorld* World = nullptr;
};
