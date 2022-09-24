// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlantData.generated.h"

class UTileEffect;

/* \/ ========== \/ *\
|  \/ UPlantData \/  |
\* \/ ========== \/ */
/**
 * All of the stats of a plant type.
 */
UCLASS()
class SYRUP_API UPlantData : public UDataAsset
{
	GENERATED_BODY()
public:
	/**
	 * Gets the name of this plant type.
	 * 
	 * @return The text to use when referring to a plant of this type.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FText GetDisplayName() const { return DisplayName; };

	/**
	 * Gets the mesh of this plant type.
	 * 
	 * @return The static mesh to use for this plant type.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE UStaticMesh* GetMesh() const { return Mesh; };

	
	/**
	 * Gets the offset of the plant mesh.
	 * 
	 * @return The location of the mesh relative to the center of the (0,0) tile.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector2D GetMeshOffset() const { return MeshOffset; };

	/**
	 * Gets the shape of this plant type.
	 * 
	 * @return A set containing all of the relative locations of the sub-tiles making up the shape of this plant type.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE TSet<FIntPoint> GetShape() const { return Shape; };

	/**
	 * Gets the max health of this plant type.
	 * 
	 * @return The number of damage points a plant of this type can take before dying.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetMaxHealth() const { return MaxHealth; };

	/**
	 * Gets the turns taken for this plant type to grow.
	 * 
	 * @return The number of turns it takes for this plant type's effects to begin taking effect.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetTimeUntilGrown() const { return TimeUntilGrown; };

	/**
	 * Gets cost to plant this plant type.
	 * 
	 * @return The amount of energy required to plant a plant of this type.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetPlantingCost() const { return PlantingCost; };

	/**
	 * Gets the range of this plant type's effects.
	 * 
	 * @return The scale applied to the shape of this plant type to get all effected locations of this plant type's effects.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetRange() const { return Range; };

	/**
	 * Gets the effects that this plant type will have.
	 * 
	 * @return An array containing all of the effects that this plant type will have inside its range.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<UTileEffect*> GetEffects() const { return Effects; };

protected:
	//The text to use when referring to a plant of this type.
	UPROPERTY(EditDefaultsOnly)
	FText DisplayName = FText();

	//The static mesh to use for this plant type.
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* Mesh = nullptr;

	//The location of the mesh relative to the center of the (0,0) tile.
	UPROPERTY(EditDefaultsOnly)
	FVector2D MeshOffset = FVector2D::ZeroVector;

	//A set containing all of the relative locations of the sub-tiles making up the shape of this plant type.
	UPROPERTY(EditDefaultsOnly)
	TSet<FIntPoint> Shape = TSet<FIntPoint>();

	//The number of damage points a plant of this type can take before dying.
	UPROPERTY(EditDefaultsOnly, Meta=(ClampMin = "1"))
	int MaxHealth = 1;

	//The number of turns it takes for this plant type's effects to begin taking effect.
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	int TimeUntilGrown = 1;

	//The amount of energy required to plant a plant of this type.
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	int PlantingCost = 1;

	//The scale applied to the shape of this plant type to get all effected locations of this plant type's effects.
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	int Range = 1;

	//An array containing all of the effects that this plant type will have inside its range.
	UPROPERTY(EditDefaultsOnly)
	TArray<UTileEffect*> Effects = TArray<UTileEffect*>();
};
/* /\ ========== /\ *\
|  /\ UPlantData /\  |
\* /\ ========== /\ */