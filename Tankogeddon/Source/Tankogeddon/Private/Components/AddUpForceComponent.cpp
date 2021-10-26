// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AddUpForceComponent.h"

// Sets default values for this component's properties
UAddUpForceComponent::UAddUpForceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAddUpForceComponent::BeginPlay()
{
	Super::BeginPlay();

	Mesh = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	
}


// Called every frame
void UAddUpForceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector UpVector = GetUpVector();

	if (Mesh->GetComponentLocation().Z < 850)
	{
		Mesh->AddForce(UpVector * ForcePower * Mesh->GetMass());
	}
	
}

