// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrashfallSaveData.generated.h"

class ATrashfallVolume;

/**
 * 
 */
USTRUCT()
struct SYRUP_API FTrashfallSaveData
{
	GENERATED_BODY()
	
public:
	FTrashfallSaveData(FIntPoint LinkedTrashLocation = FIntPoint::ZeroValue, ATrashfallVolume* TrashfallVolume = nullptr)
	{
		TrashLocation = LinkedTrashLocation;
		Volume = TrashfallVolume;
	}

	UPROPERTY()
	FIntPoint TrashLocation;

	UPROPERTY()
	ATrashfallVolume* Volume;
};