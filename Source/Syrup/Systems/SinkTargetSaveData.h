// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Syrup/Tiles/GridLibrary.h"
#include "SinkTargetSaveData.generated.h"

/**
 * 
 */
USTRUCT()
struct SYRUP_API FSinkTargetSaveData
{
	GENERATED_BODY()
	
public:
	FSinkTargetSaveData(FIntPoint TargetLocation = FIntPoint(), FName TargetSinkName = FName())
	{
		Location = TargetLocation;
		SinkName = TargetSinkName;
	}


	UPROPERTY()
	FIntPoint Location;

	UPROPERTY()
	FName SinkName;

	//Override the comparison operator
	bool operator==(const FSinkTargetSaveData& Other) const
	{
		return Location == Other.Location && SinkName == Other.SinkName;
	}
};

FORCEINLINE uint32 GetTypeHash(const FSinkTargetSaveData& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FSinkTargetSaveData));
	return Hash;
}