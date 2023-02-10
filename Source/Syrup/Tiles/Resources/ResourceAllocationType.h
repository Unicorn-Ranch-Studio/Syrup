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
	PlantHealth			UMETA(DisplayName = "Max Health"),
	PlantRange			UMETA(DisplayName = "Range"),
	PlantProduction		UMETA(DisplayName = "Production"),
	TrashDamage			UMETA(DisplayName = "Damage"),
	TrashRange			UMETA(DisplayName = "Range"),
	TrashCost			UMETA(DisplayName = "Pickup Cost")
};
/* /\ ====================== /\ *\
|  /\ EResourceAllocationType /\  |
\* /\ ====================== /\ */
