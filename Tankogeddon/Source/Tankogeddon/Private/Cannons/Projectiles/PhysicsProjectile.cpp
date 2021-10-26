// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannons/Projectiles/PhysicsProjectile.h"
#include "Components/PhysicsMovementComponent.h"
#include "InterfaceClasses/Damageable.h"

APhysicsProjectile::APhysicsProjectile()
{
	MovementComponent = CreateDefaultSubobject<UPhysicsMovementComponent>(TEXT("MovementComponent"));
}

void APhysicsProjectile::Start()
{
	Super::Start();
	MovementComponent->Velocity = GetActorForwardVector() * MoveSpeed;
	MovementComponent->SetComponentTickEnabled(true);
}

void APhysicsProjectile::Stop()
{
	MovementComponent->Velocity = FVector::ZeroVector;
	MovementComponent->SetComponentTickEnabled(false);

	Super::Stop();
}

void APhysicsProjectile::Tick(float DeltaTime)
{
	if (GetActorLocation().Z < -10000.f) Stop();
}

