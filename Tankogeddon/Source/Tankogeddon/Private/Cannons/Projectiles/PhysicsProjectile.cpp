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

void APhysicsProjectile::OnComponentHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	//Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, HitResult);

	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + FVector(0.1f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(ExpolionRange);
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	Params.TraceTag = "ExplodeTrace";

	TArray<FHitResult> AttackResult;

	FQuat Rotation = FQuat::Identity;

	GetWorld()->DebugDrawTraceTag = "ExplodeTrace";

	bool bSweepResult = GetWorld()->SweepMultiByChannel
	(
		AttackResult,
		StartPos,
		EndPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		Params
	);

	if (bSweepResult)
	{
		for (FHitResult HitRes:AttackResult)
		{
			AActor* HitActor = HitRes.GetActor();
			if (!HitActor)
			{
				continue;
			}

			IDamageable* DamageableActor = Cast<IDamageable>(HitActor);
			if (DamageableActor)
			{
				FDamageData DamageData;
				DamageData.DamageAmount = Damage;
				DamageData.Instigator = GetOwner();
				DamageData.DamageMaker = this;

				DamageableActor->TakeDamage(DamageData);
			}

			UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
			if (PrimitiveComponent)
			{
				if (PrimitiveComponent->IsSimulatingPhysics())
				{
					FVector Impusle = HitActor->GetActorLocation() - GetActorLocation();
					Impusle.Normalize();
					PrimitiveComponent->AddImpulseAtLocation(Impusle * ExpolionImpulse, HitResult.ImpactPoint);
				}
			}
		}
	}
	Stop();
}
