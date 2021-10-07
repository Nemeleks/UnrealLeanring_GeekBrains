// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannons/Cannon.h"
#include <Components/ArrowComponent.h>
#include "DrawDebugHelpers.h"
#include "Player/TankPawn.h"

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
	if (CannonType == ECannonType::FireProjectiles)
	{
		if (!bIsReadyToFireProjectiles) return;
		CurrentAmmo -= 1;
		bIsReadyToFireProjectiles = false;
		ProjectilesFire();
		GetWorld()->GetTimerManager().SetTimer(ProjectilesReloadTimerHandle, this, &ACannon::ProjectilesReload, 1.f / FireRate, false);
	}
	else if (CannonType == ECannonType::FireTrace)
	{
		if (!bIsReadyToFireTrace) return;
		CurrentAmmo -= 1;
		bIsReadyToFireTrace = false;
		if (bIsReadyToMultiplyFire)
		{
			bIsReadyToMultiplyFire = false;
			GetWorld()->GetTimerManager().SetTimer(TraceFireTimerHandle, this, &ACannon::TraceFire, 0.3f / FireRate, true);
		}	
		GetWorld()->GetTimerManager().SetTimer(TraceReloadTimerHandle, this, &ACannon::TraceReload, 5.f / FireRate, false);
	}
}
	

void ACannon::ProjectilesFire()
{
	FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
	FVector TraceEnd = TraceStart + ProjectileFireRange * ProjectileSpawnPoint->GetForwardVector();
	DrawDebugLine(GetWorld(), TraceStart,TraceEnd , FColor::Red, false, 0.5f, 0, 7.f);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Fire - Projectile"));
}

void ACannon::TraceFire()
{

	MultiplyFireCurrent += 1;
	FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
	FVector TraceEnd = TraceStart + TraceFireRange * ProjectileSpawnPoint->GetForwardVector();
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Purple, false, 0.5f, 0, 7.f);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Purple, TEXT("Fire - Trace"));

	if (MultiplyFireCurrent == MultiplyFireMaxCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(TraceFireTimerHandle);
	}
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bIsReadyToFireProjectiles = true;
	bIsReadyToFireTrace = true;
	bIsReadyToMultiplyFire = true;
	CurrentAmmo = MaxAmmo;
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

void ACannon::TraceReload()
{
	bIsReadyToFireTrace = true;
	bIsReadyToMultiplyFire = true;
	MultiplyFireCurrent = 0;
}

