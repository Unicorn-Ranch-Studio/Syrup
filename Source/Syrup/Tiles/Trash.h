// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Trash.generated.h"

/* \/ ====== \/ *\
|  \/ ATrash \/  |
\* \/ ====== \/ */
/**
 * A piece of trash on the grid that can spread, cause damage, and be picked up
 */
UCLASS()
class SYRUP_API ATrash : public ATile
{
	GENERATED_BODY()
};
/* /\ ====== /\ *\
|  /\ ATrash /\  |
\* /\ ====== /\ */