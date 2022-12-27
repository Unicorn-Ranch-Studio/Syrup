// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceAllocationType.generated.h"

 /* \/ ====================== \/ *\
 |  \/ EResourceAllocationType \/  |
 \* \/ ====================== \/ */

/**
 * In what way a resource is allocated.
 */
UENUM(BlueprintType)
enum class EResourceAllocationType : uint8
{
	NotAllocated		UMETA(DisplayName = "Not Allocated"),
	PlantHealth			UMETA(DisplayName = "Plant Health"),
	PlantRadius			UMETA(DisplayName = "Plant Radius"),
	PlantProduction		UMETA(DisplayName = "Plant Production"),
	TrashDamage			UMETA(DisplayName = "Trash Damage"),
	TrashRadius			UMETA(DisplayName = "Trash Radius"),
	TrashCost			UMETA(DisplayName = "Trash Pickup Cost")
};
/* /\ ====================== /\ *\
|  /\ EResourceAllocationType /\  |
\* /\ ====================== /\ */
