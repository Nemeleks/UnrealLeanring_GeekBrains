// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannons/Projectiles/BaseProjectile.h"
#include "Subsystems/ActolPoolSubsystem.h"
#include "InterfaceClasses/Damageable.h"
#include "InterfaceClasses/Scorable.h"
#include "Player/TankPawn.h"
#include <GameFramework/Actor.h>

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.005f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnComponentHit);
	Mesh->SetHiddenInGame(true);

}

void ABaseProjectile::Start()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	StartPosition = GetActorLocation();
	Mesh->SetHiddenInGame(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABaseProjectile::Stop()
{
	GetScoreOnKill.Clear();
	PrimaryActorTick.SetTickFunctionEnable(false);
	Mesh->SetHiddenInGame(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UActolPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActolPoolSubsystem>();
	if (Pool->IsActorInPool(this))
	{
		Pool->MoveActorToPool(this);
	}
	else
	{
		Destroy();		
	}
}

void ABaseProjectile::OnComponentHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s"), *GetName(), *OtherActor->GetName());

	if (OtherActor == GetInstigator())
	{
		Stop();
		return;
	}

	if (bIsExlosiveProjectile)
	{
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
			for (FHitResult HitRes : AttackResult)
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
	}

	if (OtherComp->IsSimulatingPhysics())
	{
		FVector Impulse = Mass * MoveSpeed * GetActorForwardVector();
		OtherComp->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint);
	}

	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	else if (IDamageable* Damageable = Cast<IDamageable>(OtherActor))
	{
		FDamageData DamageData;
		DamageData.DamageAmount = Damage;
		DamageData.Instigator = GetInstigator();
		DamageData.DamageMaker = this;
		Damageable->TakeDamage(DamageData);

		if (IScorable* Scorable = Cast<IScorable>(OtherActor))
		{
			if (OtherActor->IsActorBeingDestroyed())
			{
				if (GetScoreOnKill.IsBound())
				{
					GetScoreOnKill.Broadcast(Scorable->GetScoreForKill());
				}
			}
		}
	}
	Stop ();
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NewLocation, true);

	if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange)
	{
		Stop();
	}
}

