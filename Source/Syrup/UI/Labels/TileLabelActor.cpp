// Fill out your copyright notice in the Description page of Project Settings.


#include "TileLabelActor.h"

#include "Components/WidgetComponent.h"
#include "TileLabelContainer.h"
#include "Syrup/Tiles/GridLibrary.h"
#include "Syrup/Tiles/Tile.h"
#include "Syrup/Systems/SyrupGameMode.h"

// Sets default values
ATileLabelActor::ATileLabelActor()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root Component"));

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("Widget Component"));
    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    WidgetComponent->SetDrawAtDesiredSize(true);
    WidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

/**
 * Creates a tile label actor at the given tile location with the given label.
 *
 * @param WorldContextObject - Some object in the world to create the label in.
 * @param ContainerClass - The class of container to create.
 * @param Location - The grid location to spawn the label actor at.
 *
 * @return The label container at this grid location.
 */
UTileLabelContainer* ATileLabelActor::Create(const UObject* WorldContextObject, const TSubclassOf<UTileLabelContainer> ContainerClass, const FIntPoint Location)
{
    ATileLabelActor* CreatedActor = WorldContextObject->GetWorld()->SpawnActor<ATileLabelActor>(ATileLabelActor::StaticClass(), FTransform(UGridLibrary::GridLocationToWorldLocation(Location)));

    CreatedActor->WidgetComponent->SetWidgetClass(ContainerClass);
    UTileLabelContainer* CreatedContainer = Cast<UTileLabelContainer>(CreatedActor->WidgetComponent->GetWidget());
    CreatedContainer->Location = Location;
    CreatedActor->GridLocation = Location;

    ASyrupGameMode::GetTileEffectTriggerDelegate(WorldContextObject).AddDynamic(CreatedActor, &ATileLabelActor::ReceiveEffectTrigger);
    CreatedContainer->OnContainerEmptied.AddUObject(CreatedActor, &ATileLabelActor::DestroyLabel);
    CreatedActor->Snap();

    return CreatedContainer;
}

/**
 * Updates the label to snap/unsnap from tiles.
 *
 * @param TriggerType - The of trigger that was activated.
 * @param LocationsToTrigger - The Locations where the trigger applies an effect. If this is empty all effect locations will be effected.
 */
void ATileLabelActor::ReceiveEffectTrigger(const ETileEffectTriggerType TriggerType, const TSet<FIntPoint>& LocationsToTrigger)
{
    switch (TriggerType)
    {
    case ETileEffectTriggerType::PlantSpawned:
    case ETileEffectTriggerType::TrashSpawned:
        if (LocationsToTrigger.Contains(GridLocation))
        {
            Snap();
        }
        break;

    case ETileEffectTriggerType::PlantKilled:
    case ETileEffectTriggerType::TrashPickedUp:
        if (LocationsToTrigger.Contains(GridLocation))
        {
            WidgetComponent->SetRelativeLocation(FVector::ZeroVector);
        }
        break;

    default:
        break;
    }
}

/**
 * Snaps this label actor to the tile at its location if it exists
 */
void ATileLabelActor::Snap()
{
    ATile* TileToSnapTo;
    if (UGridLibrary::OverlapGridLocation(this, GridLocation, TileToSnapTo, TArray<AActor*>()))
    {
        WidgetComponent->SetWorldLocation(TileToSnapTo->LabelRoot->GetComponentLocation());
    }
    else
    {
        WidgetComponent->SetRelativeLocation(FVector::ZeroVector);
    }
}

void ATileLabelActor::DestroyLabel()
{
    Destroy();
}