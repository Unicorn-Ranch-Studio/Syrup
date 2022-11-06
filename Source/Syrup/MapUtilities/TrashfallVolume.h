// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Syrup/Tiles/Effects/TileEffectTrigger.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrashfallVolume.generated.h"

class ATrash;
class UBoxComponent;

/* \/ ================ \/ *\
|  \/ ATrashfallVolume \/  |
\* \/ ================ \/ */

/**
 * Serves as a spawn point for trash.
 */
UCLASS(HideCategories = ("Navigation", "HLOD", "Physics", "Collision", "Rendering", "Tags", "Cooking", "Replication", "Input", "Actor", "Mobile", "RayTracing", "AssetUserData"))
class SYRUP_API ATrashfallVolume : public AActor
{
	GENERATED_BODY()
	
public:
	/**
	 * Initializes box component.
	 */
	ATrashfallVolume();

	/**
	 * Respawns all trash spawned by this
	 */
	UFUNCTION(CallInEditor, Category = "Trashfall")
	void ResetTrashLocations();

	//The type of trash that will fall in this volume.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Trashfall", Meta=(AllowAbstract = "false"))
	TSubclassOf<ATrash> TrashType;

	//The number of trash that this trashfall volume will attempt to maintain.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Trashfall", Meta = (ClampMin = "1"))
	int NumToMaintain = 3;

	//The number of turns it takes for this volume to spawn a piece of trash.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Trashfall", Meta = (ClampMin = "0"))
	int TurnsBetweenSpawns = 1;

	//Whether or not this volume should already be at capacity when the game starts.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Trashfall")
	bool bStartWithTrash = true;
protected:
	//The area in which this will attempt to spawn trash.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* SpawnArea;

private:
	/**
	 * Links this to the game mode for receiving trigger events.
	 */
	virtual void BeginPlay() override;

	/**
	 * Destroys trash spawned by this.
	 */
	virtual void Destroyed() override;

	/**
	 * Spawns the appropriate number of trash.
	 *
	 * @param Transform - The new transform of the volume.
	 */
	virtual void OnConstruction(const FTransform& Transform) override;

	/**
	 * Activates the trashfall if trigger type is trash spreads.
	 * 
	 * @param TriggerType - The of trigger that was activated.
	 * @param LocationsToTrigger - The Locations where the trigger applies an effect.
	 */
	UFUNCTION()
	void ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& LocationsToTrigger);

	/**
	 * Called when one of the trash spawned by this is destroyed.
	 *
	 * @param DestroyedActor - The actor that was destroyed.
	 */
	UFUNCTION()
	FORCEINLINE void ReciveTrashDestoryed(AActor* DestroyedActor) { NumTrash--;};

	

	/**
	 * Spawns a single trash inside the spawn area.
	 * 
	 * @param bAttachTrash - Whether or not to attach the trash to this.
	 * 
	 * @return Whether or not a trash was sucessfuly spawned.
	 */
	UFUNCTION()
	bool SpawnTrash(bool bAttachTrash = false);

	//The number of trash existing trash that has been spawned by this.
	UPROPERTY()
	int NumTrash = 0;

	//The approximate number of tiles inside this volume.
	UPROPERTY()
	int NumTiles = 0;
	
	//The number of turns until this will spawn another trash.
	UPROPERTY()
	int TurnsUntilSpawn = 1;

	//Locations known to block trash spawning.
	UPROPERTY()
	TSet<FIntPoint> BadLocations = TSet<FIntPoint>();
};
