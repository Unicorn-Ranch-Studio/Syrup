// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Syrup/Tiles/GridLibrary.h"
#include "FaucetSaveData.generated.h"

/**
 * 
 */
USTRUCT()
struct SYRUP_API FFaucetSaveData
{
	GENERATED_BODY()
	
public:
	FFaucetSaveData(FIntPoint FaucetLocation = FIntPoint::ZeroValue, FIntPoint TargetSinkLocation = FIntPoint::ZeroValue, FName TargetSinkName = FName())
	{
		Location = FaucetLocation;
		TargetLocation = TargetSinkLocation;
		SinkName = TargetSinkName;
	}

	UPROPERTY()
	FIntPoint Location;

	UPROPERTY()
	FIntPoint TargetLocation;

	UPROPERTY()
	FName SinkName;
};