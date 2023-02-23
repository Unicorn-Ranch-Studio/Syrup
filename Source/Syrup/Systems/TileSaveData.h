// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SinkTargetSaveData.h"

#include "CoreMinimal.h"
#include "Syrup/Tiles/GridLibrary.h"
#include "TileSaveData.generated.h"

/**
 * 
 */
USTRUCT()
struct SYRUP_API FTileSaveData
{
	GENERATED_BODY()
	
public:
	FTileSaveData(FGridTransform Transform = FGridTransform(), TSubclassOf<ATile> Class = TSubclassOf<ATile>(), TArray<FSinkTargetSaveData> TargetsOfTileFaucet = TArray<FSinkTargetSaveData>())
	{
		TileTransfrom = Transform;
		TileClass = Class;
		TileFaucetTargets = TargetsOfTileFaucet;
	}

	UPROPERTY()
	FGridTransform TileTransfrom;

	UPROPERTY()
	TSubclassOf<ATile> TileClass;

	UPROPERTY()
	TArray<FSinkTargetSaveData> TileFaucetTargets;
};