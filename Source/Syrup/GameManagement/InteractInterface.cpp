// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractInterface.h"

// this file is stoopid and I wish it wasn't necessary... dang you, linker

/* \/ =================== \/ *\
|  \/ IInteractInterface  \/  |
\* \/ =================== \/ */

/**
 * Handles what actually happens when this is interacted with
 */
void IInteractInterface::Interact(){}

/**
 * Handles what happens when this becomes interactable (likely due to the player coming within range)
 */
void IInteractInterface::BecomeInteractable(){}

/**
 * Handles what happens when this is no longer interactable (likely due to the player leaving the range)
 */
void IInteractInterface::BecomeNotInteractable(){}

/* /\ ================== /\ *\
|  /\ IInteractInterface /\  |
\* /\ ================== /\ */