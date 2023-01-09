// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceSink.h"

 /* \/ ============ \/ *\
 |  \/ ResourceSink \/  |
 \* \/ ============ \/ */

UResourceSink* UResourceSink::AddResourceSinkComponent(AActor* Owner, FResourceSinkAmountData SinkAmountData, const FSinkAmountUpdateDelegate& UpdateCallback, const FSinkLocationsDelegate& GetLocations, const FSinkAmountDelegate& GetAmount)
{
	UResourceSink* NewSink = Cast<UResourceSink>(Owner->AddComponentByClass(StaticClass(), false, FTransform(), false));
	NewSink->AmountData = SinkAmountData;
	NewSink->OnAmountChanged = UpdateCallback;
	NewSink->GetAllocationLocations = GetLocations;
	NewSink->GetAllocatedAmount = GetAmount;

	return NewSink;
}
/**
 * Undoes the effect of a resource that was sunk in this.
 *
 * @param FreedResource - The resource that was freed.
 */
void UResourceSink::AllocateResource(UResource* FreedResource)
{

}

/**
 * Undoes the effect of a resource that was sunk in this.
 *
 * @param FreedResource - The resource that was freed.
 */
void UResourceSink::FreeResource(UResource* FreedResource)
{

}

/* /\ ============ /\ *\
|  /\ ResourceSink /\  |
\* /\ ============ /\ */