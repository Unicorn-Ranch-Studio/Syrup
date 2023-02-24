// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTakenSaveData.generated.h"

/**
 * 
 */
USTRUCT()
struct SYRUP_API FDamageTakenSaveData
{
	GENERATED_BODY()
	
public:
	FDamageTakenSaveData(FIntPoint DamagedLocation = FIntPoint::ZeroValue, int DamagedAmount = 0)
	{
		Location = DamagedLocation;
		Amount = DamagedAmount;
	}

	UPROPERTY()
	FIntPoint Location;

	UPROPERTY()
	int Amount;
};