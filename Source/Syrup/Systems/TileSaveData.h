// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	UPROPERTY()
	FGridTransform TileTransfrom;

	UPROPERTY()
	TSubclassOf<ATile> TileClass;

	TArray<TTuple<FIntPoint, FName>> SinkTargets;
};
