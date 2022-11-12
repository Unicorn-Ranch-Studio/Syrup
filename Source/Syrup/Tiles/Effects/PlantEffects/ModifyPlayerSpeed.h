// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Syrup/Tiles/Effects/VolumetricEffect.h"
#include "ModifyPlayerSpeed.generated.h"

/* \/ ================== \/ *\
|  \/ UModifyPlayerSpeed \/  |
\* \/ ================== \/ */
/**
 * When the player enters the effected area they will incur a change in movement speed.
 */
UCLASS(ClassGroup = (TileEffects), Meta = (BlueprintSpawnableComponent))
class SYRUP_API UModifyPlayerSpeed : public UVolumetricEffect
{
	GENERATED_BODY()
public:
	//This movement speed will be added to the players walk speed when they enter the effected area.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MovementSpeedChange = 100;

protected:
	/**
	 * Overrides the triggers variable.
	 */
	UModifyPlayerSpeed();

	/**
	 * Applies the effect of this volume when overlap is begun.
	 * 
	 * @param OtherActor - The actor that entered the volume.
	 */
	virtual void OnBeginOverlap(AActor* OtherActor) override;

	/**
	 * Applies the effect of this volume when overlap is ended.
	 * 
	 * @param OtherActor - The actor that left the volume.
	 */
	virtual void OnEndOverlap(AActor* OtherActor) override;

	/**
	 * Gets the collision channels that this volume will overlap.
	 * 
	 * @return A bitwise integer specifying which channels will be overlapped.
	 */
	virtual TSet<TEnumAsByte<ECollisionChannel>> GetOverlappedChannels() const override;
};
/* /\ ================== /\ *\
|  /\ UModifyPlayerSpeed /\  |
\* /\ ================== /\ */
