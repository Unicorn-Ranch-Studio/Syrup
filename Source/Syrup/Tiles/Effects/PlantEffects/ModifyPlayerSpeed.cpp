// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifyPlayerSpeed.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


/**
 * Overrides the triggers variable.
 */
UModifyPlayerSpeed::UModifyPlayerSpeed()
{
	AffectTriggers.Add(ETileEffectTriggerType::OnActivated);
	UnaffectTriggers.Add(ETileEffectTriggerType::OnDeactivated);
}

/**
 * Applies the effect of this volume when overlap is begun.
 *
 * @param OtherActor - The actor that entered the volume.
 */
void UModifyPlayerSpeed::OnBeginOverlap(AActor* OtherActor)
{
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (IsValid(Player))
	{
		Player->GetCharacterMovement()->MaxWalkSpeed += MovementSpeedChange;
	}
}

/**
 * Applies the effect of this volume when overlap is ended.
 *
 * @param OtherActor - The actor that left the volume.
 */
void UModifyPlayerSpeed::OnEndOverlap(AActor* OtherActor)
{
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (IsValid(Player))
	{
		Player->GetCharacterMovement()->MaxWalkSpeed -= MovementSpeedChange;
	}
}

/**
 * Gets the collision channels that this volume will overlap.
 *
 * @return A bitwise integer specifying which channels will be overlapped.
 */
TSet<TEnumAsByte<ECollisionChannel>> UModifyPlayerSpeed::GetOverlappedChannels() const
{
	TSet<TEnumAsByte<ECollisionChannel>> ReturnValue = TSet<TEnumAsByte<ECollisionChannel >>();
	ReturnValue.Add(ECC_Pawn);
	return ReturnValue;
}