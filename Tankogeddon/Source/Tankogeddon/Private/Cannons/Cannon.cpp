// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannons/Cannon.h"
#include <Components/ArrowComponent.h>
#include "DrawDebugHelpers.h"
#include "Player/TankPawn.h"
#include "Cannons/Projectiles/BaseProjectile.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComponent = CreateDefaultSubobject < USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPount"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire()
{
	if (CurrentAmmo < 1) return;
	if (CannonType == ECannonType::ProjectileCannon)
	{
		if (!bIsReadyToFireProjectiles) return;
		CurrentAmmo -= 1;
		bIsReadyToFireProjectiles = false;
		ProjectilesFire();
		GetWorld()->GetTimerManager().SetTimer(ProjectilesReloadTimerHandle, this, &ACannon::ProjectilesReload, 1.f / FireRate, false);
	}
	else if (CannonType == ECannonType::TraceCannon)
	{
		if (!bIsReadyToFireProjectiles) return;
		CurrentAmmo -= 1;
		bIsReadyToFireProjectiles = false;
		TraceFire();
		GetWorld()->GetTimerManager().SetTimer(ProjectilesReloadTimerHandle, this, &ACannon::ProjectilesReload, 1.f / FireRate, false);
	}
	else if (CannonType == ECannonType::Machinegun)
	{

	}
}
	

void ACannon::ProjectilesFire()
{
	ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Fire - Projectile"));
	if (Projectile)
	{
		Projectile-> Start();
	}
}

void ACannon::TraceFire()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Purple, TEXT("Fire Trace"));

	FHitResult HitResult;
	FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
	FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * TraceFireRange;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Red, false, 0.5f, 0, 5.f);
		if (HitResult.Actor.IsValid() && HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
		{
			HitResult.Actor->Destroy();
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
	}
}

void ACannon::MultiplyFire()
{

	MultiplyFireCurrent += 1;
	if (MultiplyFireCurrent == MultiplyFireMaxCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(TraceFireTimerHandle);
	}
	ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Fire - Projectile"));
	if (Projectile)
	{
		Projectile->Start();
	}


}

void ACannon::AltFire()
{
	if (CurrentAmmo < 1) return;
	if (CannonType == ECannonType::ProjectileCannon)
	{
		if (!bIsReadyToFireProjectiles) return;
		CurrentAmmo -= 1;
		//bIsReadyToAltFire = false;
		bIsReadyToFireProjectiles = false;
		if (bIsReadyToMultiplyFire)
		{
			bIsReadyToMultiplyFire = false;
			GetWorld()->GetTimerManager().SetTimer(TraceFireTimerHandle, this, &ACannon::MultiplyFire, 0.3f / FireRate, true);
		}
		GetWorld()->GetTimerManager().SetTimer(TraceReloadTimerHandle, this, &ACannon::MultiplyFireReload, 3.f / FireRate, false);
	}
	else if (CannonType == ECannonType::TraceCannon)
	{

	}
	else if (CannonType == ECannonType::Machinegun)
	{

	}
	
}

void ACannon::AddAmmo(int32 Amount)
{
	CurrentAmmo += Amount;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bIsReadyToFireProjectiles = true;
	bIsReadyToAltFire = true;
	bIsReadyToMultiplyFire = true;

}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ACannon::ProjectilesReload()
{
	bIsReadyToFireProjectiles = true;
}

void ACannon::MultiplyFireReload()
{
	//bIsReadyToAltFire = true;
	bIsReadyToFireProjectiles = true;
	bIsReadyToMultiplyFire = true;
	MultiplyFireCurrent = 0;
}

