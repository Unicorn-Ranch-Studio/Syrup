// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResourceType.generated.h"

 /* \/ ============= \/ *\
 |  \/ EResourceType \/  |
 \* \/ ============= \/ */

/**
 * A kind of resource.
 */
UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Any			UMETA(DisplayName = "Any Resource"),
	Water		UMETA(DisplayName = "Water"),
	Sugar		UMETA(DisplayName = "Sugar"),
	Nitrogen	UMETA(DisplayName = "Nitrogen")
};
/* /\ ============= /\ *\
|  /\ EResourceType /\  |
\* /\ ============= /\ */
