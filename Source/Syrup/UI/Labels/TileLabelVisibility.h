// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/* \/ ==================== \/ *\
|  \/ ETileLabelVisibility \/  |
\* \/ ==================== \/ */
/**
 * The state in which a tile label will be visible.
 */
UENUM(BlueprintType)
enum class ETileLabelVisibility : uint8
{
	//Label will appear when its location is or one of its source locations are selected.
	LocationOrSource	UMETA(DisplayName = "Location or Source Locations Selected"),
	//Label will appear when its location is selected.
	Location			UMETA(DisplayName = "Location Selected"),
	//Label will appear when one of its source locations are selected.
	Source				UMETA(DisplayName = "Source Locations Selected"),
	//Label will always appear.
	Always				UMETA(DisplayName = "Always Visible"),
	//Label will never appear.
	Never				UMETA(DisplayName = "Never Visible")
};
/* /\ ==================== /\ *\
|  /\ ETileLabelVisibility /\  |
\* /\ ==================== /\ */