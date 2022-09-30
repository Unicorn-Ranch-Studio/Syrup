// Fill out your copyright notice in the Description page of Project Settings.


#include "Trash.h"

#include "Syrup/Systems/SyrupGameMode.h"
#include "Effects/ApplyField.h"
#include "Effects/DamagePlants.h"
#include "Components/InstancedStaticMeshComponent.h"

/* \/ ====== \/ *\
|  \/ ATrash \/  |
\* \/ ====== \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Creates the mesh and effect components.
 */
ATrash::ATrash()
{
	//Init Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Trash Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->AttachToComponent(SubtileMesh, FAttachmentTransformRules::KeepRelativeTransform);

	//Init Grass
	GoopComponent = CreateDefaultSubobject<UApplyField>(FName("Goop Zone"));
	GoopComponent->FieldType = EFieldType::Damage;

	//Init Prevent Trash Spawn
	DamageComponent = CreateDefaultSubobject<UDamagePlants>(FName("Damage Zone"));
	UpdateDamage();

	//Get Trash Mat
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatRef(TEXT("/Game/Tiles/Trash/MI_Trash.MI_Trash"));
	TileMaterial = MatRef.Object;
	check(TileMaterial != nullptr);
}

/**
 * Binds effect triggers.
 */
void ATrash::BeginPlay()
{
	Super::BeginPlay();

	TSet<ATile*> InRangeTiles = TSet<ATile*>();
	TArray<AActor*> IngnoredActors = TArray<AActor*>();
	IngnoredActors.Add(this);

	if (UGridLibrary::OverlapShape(GetWorld(), GetEffectLocations(), InRangeTiles, IngnoredActors))
	{
		for (ATile* EachInRangeTile : InRangeTiles)
		{
			if (IsValid(Cast<ATrash>(EachInRangeTile)))
			{
				NumTrashInRadius++;
			}
		}
	}

	ReceiveEffectTrigger(ETileEffectTriggerType::Persistent, TSet<FIntPoint>());
	ASyrupGameMode::GetTileEffectTriggerDelegate(this).AddDynamic(this, &ATrash::ReceiveEffectTrigger);
}

/**
 * Initializes Damage, Range, and TimeUntilSpread; and sets the appropriate mesh.
 *
 * @param Transform - The new transform of the trash.
 */
void ATrash::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	MeshComponent->SetStaticMesh(GetMesh());
	Range = GetRange();
	Shape.Add(FIntPoint::ZeroValue);
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ------------ *\
\* \/ Damage \/ */

/**
 * Updates the damage that this trash will deal.
 *
 * @param AmountAdded - The number of damage points to add to the current damage of this trash. May be a negative number
 */
void ATrash::UpdateDamage(int AmountAdded)
{
	Damage = FMath::Max(0, Damage + AmountAdded);
	DamageComponent->SetDamage(Damage);
}

/* /\ Damage /\ *\
\* ------------ */

/* --------------- *\
\* \/ Spreading \/ */

/**
 * Spawns another trash of the same as this.
 */
void ATrash::Spread()
{
	if (TimeUntilSpread-- <= 1 && NumTrashInRadius < MaxTrashDensity)
	{
		TimeUntilSpread = GetInitialTimeUntilSpread();

		TArray<FIntPoint> EffectLocations = GetEffectLocations().Difference(GetSubTileLocations()).Array();

		FTransform SpawnTransform = UGridLibrary::GridTransformToWorldTransform(FGridTransform(EffectLocations[FMath::RandHelper(EffectLocations.Num())]));
		SpawnTransform.SetRotation(FQuat(FVector(0, 0, 1), FMath::FRandRange(0, TWO_PI)));
		FActorSpawnParameters Params = FActorSpawnParameters();
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ATrash* SpawnedTrash = GetWorld()->SpawnActor<ATrash>(GetClass(), SpawnTransform, Params);

		if (IsValid(SpawnedTrash))
		{
			TSet<FIntPoint> SpawnedLocations = SpawnedTrash->GetSubTileLocations();
			FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
			TraceParams.AddIgnoredActor(SpawnedTrash);

			for (FIntPoint EachSubtileLocation : SpawnedLocations)
			{
				FHitResult Result;
				FVector WorldLocation = UGridLibrary::GridTransformToWorldTransform(FGridTransform(EachSubtileLocation)).GetTranslation();
				if (GetWorld()->LineTraceSingleByChannel(Result, WorldLocation, WorldLocation + FVector(0,0,-0.1), ECollisionChannel::ECC_GameTraceChannel2, TraceParams))
				{
					SpawnedTrash->Destroy();
					break;
				}
			}

			if (IsValid(SpawnedTrash))
			{
				ASyrupGameMode::GetTileEffectTriggerDelegate(GetWorld()).Broadcast(ETileEffectTriggerType::TrashSpawned, SpawnedLocations);
			}
		}
		DrawDebugDirectionalArrow(GetWorld(), SubtileMesh->GetComponentLocation(), SpawnTransform.GetLocation(), 80, IsValid(SpawnedTrash) ? FColor::Purple : FColor::Blue, false, 4, 0U, IsValid(SpawnedTrash) ? 10 : 1);
	}
}

/* /\ Spreading /\ *\
\* --------------- */

/* ------------ *\
\* \/ Effect \/ */

/**
 * Activates the appropriate effects given the trigger.
 *
 * @param TriggerType - The type of trigger that was activated.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
 */
void ATrash::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& LocationsToTrigger)
{
	TSet<FIntPoint> EffectedLocations = GetEffectLocations();
	TSet<FIntPoint> TriggeredLocations = LocationsToTrigger.IsEmpty() ? EffectedLocations : LocationsToTrigger.Intersect(EffectedLocations);

	if(TriggerType == ETileEffectTriggerType::TrashSpread)
	{
		FTimerHandle Handle = FTimerHandle();
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ATrash::Spread, FMath::FRand(), false);
	}
	else if (!TriggeredLocations.IsEmpty())
	{
		if (TriggerType == ETileEffectTriggerType::TrashSpawned)
		{
			NumTrashInRadius++;
		}
		else if (TriggerType == ETileEffectTriggerType::TrashPickedUp)
		{
			NumTrashInRadius--;
		}
	}

	TInlineComponentArray<UActorComponent*> Components = TInlineComponentArray<UActorComponent*>();
	GetComponents(UTileEffect::StaticClass(), Components);
	for (UActorComponent* EachComponent : Components)
	{
		Cast<UTileEffect>(EachComponent)->ActivateEffect(TriggerType, EffectedLocations);
	}
}

/**
 * Gets the locations where the effects of this trash will apply.
 *
 * @return A set of all locations where the effects of this trash will apply.
 */
TSet<FIntPoint> ATrash::GetEffectLocations() const
{
	return UGridLibrary::ScaleShapeUp(GetSubTileLocations(), Range);
}

/* /\ Effect /\ *\
\* ------------ */

/* /\ ====== /\ *\
|  /\ ATrash /\  |
\* /\ ====== /\ */