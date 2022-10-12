// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TileLabelComponent.generated.h"

class UTileLabel;

/* \/ =================== \/ *\
|  \/ UTileLabelComponent \/  |
\* \/ =================== \/ */
/**
 * Registers a tile label at its location.
 */
UCLASS(ClassGroup=(Custom), Meta=(BlueprintSpawnableComponent))
class SYRUP_API UTileLabelComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	/**
	 * Applies any changes made to Label
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateLabel();

	//The label to register.
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTileLabel> Label = nullptr;

protected:
	/**
	 * Registers a tile label at its location.
	 */
	virtual void BeginPlay() override;
};
/* /\ =================== /\ *\
|  /\ UTileLabelComponent /\  |
\* /\ =================== /\ */