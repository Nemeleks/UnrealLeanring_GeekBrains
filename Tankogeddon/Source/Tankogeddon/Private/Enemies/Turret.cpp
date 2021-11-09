  // Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Turret.h"
#include <Components/ArrowComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include "Cannons/Cannon.h"
#include <Kismet/KismetMathLibrary.h>
#include "Components/HealthComponent.h"
#include <DrawDebugHelpers.h>

// Sets default values
ATurret::ATurret()
{
 
	PrimaryActorTick.TickInterval = 0.005f;

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (Cannon->IsMortair())
	{
		float MaxAngle = FMath::DegreesToRadians(45);
		float MaxCos = FMath::Cos(MaxAngle);
		float MaxSin = FMath::Sin(MaxAngle);

		float speed = Cannon->GetProjectileMovementSpeed();
		float initial_height = Cannon->GetProgectileSpawnPointLocation().Z;
		float gravity = -GetWorld()->GetGravityZ();
		float MaxRange = (speed * MaxCos / gravity) * (speed * MaxSin + FMath::Sqrt(speed * speed * MaxSin * MaxSin + 2 * gravity * initial_height));
		TargetingRange = MaxRange;

	}
}

void ATurret::Destroyed()
{
	Super::Destroyed();

	if (Cannon)
	{
		Cannon->Destroy();
	}
}

void ATurret::Targeting()
{

	

	FHitResult HitResult;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = PlayerPawn->GetActorLocation();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("AITrace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams) && HitResult.Actor != PlayerPawn)
	{
		return;
	}

	if (IsPlayerInRange())
	{
		RotateToPlayer();
		if (Cannon->IsMortair())
		{
			float angle = FMath::DegreesToRadians(90.f + Cannon->GetCannonPitchRotation());
			float cos = FMath::Cos(angle);
			float sin = FMath::Sin(angle);

			float speed = Cannon->GetProjectileMovementSpeed();

			float initial_height = Cannon->GetProgectileSpawnPointLocation().Z;
			float gravity = -GetWorld()->GetGravityZ();

			float range = (speed * cos / gravity) * (speed * sin + FMath::Sqrt(speed * speed * sin * sin + 2 * gravity * initial_height));


			FVector EndPosit = Cannon->GetProgectileSpawnPointLocation() + Cannon->GetActorForwardVector() * range;
			EndPosit.Z = 0.f;

			DrawDebugSphere(GetWorld(), EndPosit, 20.f, 30, FColor::Purple, false, 0.1f, 0, 5.f);

			if (FVector::Distance(EndPosit, GetActorLocation()) > FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()))
			{
				Cannon->LiftCannon(1.f);
			}
			else if (FVector::Distance(EndPosit, GetActorLocation()) < FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()))
			{
				Cannon->LiftCannon(-1.f);
			}
		}
	}

	

	if (CanFire() && Cannon )
	{
		Fire();
	}
}

void ATurret::RotateToPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrentRotation = TurretMeshComponent->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMeshComponent->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
}

bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire()
{
	FVector TargetingDirection = TurretMeshComponent->GetForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirectionToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer)));
	return AimAngle <= Accuracy;
}


// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Targeting();


}

void ATurret::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

