// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldsManager.generated.h"

class ATile;

/* \/ ========== \/ *\
|  \/ EFieldType \/  |
\* \/ ========== \/ */
UENUM(BlueprintType)
enum class EFieldType : uint8
{
	Protection	UMETA(DisplayName = "Protection Radius"),
	Damage		UMETA(DisplayName = "Damage Radius")
};
/* /\ ========== /\ *\
|  /\ EFieldType /\  |
\* /\ ========== /\ */

/* \/ ============== \/ *\
|  \/ AFieldsManager \/  |
\* \/ ============== \/ */
/**
 * An object for managing and rendering the fields in a level.
 * Will also function as a ground plane.
 */
UCLASS()
class SYRUP_API AFieldsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AFieldsManager();

	/**
	 * Edits a field's strength in the given area.
	 * 
	 * @param FieldType - The type of the field. Fields of differnt types have independent strengths.
	 * @param Strength - The value to add to the field strength. Note: Field values clamped to >= 0.
	 * @param Locations - The locations to change the feild strength in.
	 */
	void AddFieldStrength(const EFieldType FieldType, const int Strength, const TSet<FIntPoint> Locations);

	/**
	 * Queries a field's strength in the given location.
	 *
	 * @param Type - The type of the field.
	 * @param Location - The locations to change the feild strength in.
	 * @return The strength of the field at the given location. Field strength will be 0 if the field is not present.
	 */
	int GetFieldStrengthAtLocation(const EFieldType FieldType, const FIntPoint Location) const;

	/**
	 * Gets the set of all tiles in a given field.
	 *
	 * @param Type - The type of the field to query.
	 * @return The set of all tiles in a given field.
	 */
	TSet<ATile*> GetTilesInField(const EFieldType FieldType) const;
};
/* /\ ============== /\ *\
|  /\ AFieldsManager /\  |
\* /\ ============== /\ */