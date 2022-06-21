// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

/* \/ ================== \/ *\
|  \/ UInteractInterface \/  |
\* \/ ================== \/ */

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/* /\ ================== /\ *\
|  /\ UInteractInterface /\  |
\* /\ ================== /\ */

/* \/ =================== \/ *\
|  \/ IInteractInterface  \/  |
\* \/ =================== \/ */

/**
 * Interface for interaction
 */
class SYRUP_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * Handles what actually happens when this is interacted with
	 */
	UFUNCTION()
	virtual void Interact();

	/**
	 * Handles what happens when this becomes interactable (likely due to the player coming within range)
	 */
	UFUNCTION()
	virtual void BecomeInteractable();

	/**
	 * Handles what happens when this is no longer interactable (likely due to the player leaving the range)
	 */
	UFUNCTION()
	virtual void BecomeNotInteractable();

};

/* /\ ================== /\ *\
|  /\ IInteractInterface /\  |
\* /\ ================== /\ */