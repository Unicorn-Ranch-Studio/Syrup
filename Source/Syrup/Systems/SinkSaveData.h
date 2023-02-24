// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SinkSaveData.generated.h"

/**
 * 
 */
USTRUCT()
struct SYRUP_API FSinkSaveData
{
	GENERATED_BODY()
	
public:
	FSinkSaveData(FIntPoint ResourceSinkLocation = FIntPoint::ZeroValue, FName ResourceSinkName = FName(), int SinkStoredAmount = 0)
	{
		Location = ResourceSinkLocation;
		Name = ResourceSinkName;
		StoredAmount = SinkStoredAmount;
	}

	UPROPERTY()
	int StoredAmount;

	UPROPERTY()
	FIntPoint Location;

	UPROPERTY()
	FName Name;
};