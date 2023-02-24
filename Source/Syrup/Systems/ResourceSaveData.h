// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Syrup/Tiles/Resources/ResourceType.h"

#include "CoreMinimal.h"
#include "ResourceSaveData.generated.h"

/**
 * 
 */
USTRUCT()
struct SYRUP_API FResourceSaveData
{
	GENERATED_BODY()
	
public:
	FResourceSaveData(FIntPoint ResourceFaucetLocation = FIntPoint::ZeroValue, FIntPoint ResourceSinkLocation = FIntPoint::ZeroValue, FName ResourceSinkName = FName(), EResourceType ResourceType = EResourceType::Any)
	{
		FaucetLocation = ResourceFaucetLocation;
		SinkLocation = ResourceSinkLocation;
		SinkName = ResourceSinkName;
		Type = ResourceType;
	}

	UPROPERTY()
	EResourceType Type;

	UPROPERTY()
	FIntPoint FaucetLocation;

	UPROPERTY()
	FIntPoint SinkLocation;

	UPROPERTY()
	FName SinkName;
};