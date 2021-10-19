// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/TankFactory.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include "TimerManager.h"
#include "Components/HealthComponent.h"
#include "Player/TankPawn.h"

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(RootComponent);

	DestroyedBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestroyedBuildingMesh"));
	DestroyedBuildingMesh->SetupAttachment(RootComponent);
	

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("TankSpawnPoint"));
	TankSpawnPoint->SetupAttachment(RootComponent);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	HitCollider->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATankFactory::OnHealthChanged);
	HealthComponent->OnDie.AddDynamic(this, &ATankFactory::OnDie);
}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	bIsFactoryAlive = true;
	DestroyedBuildingMesh->SetHiddenInGame(true);

	GetWorld()->GetTimerManager().SetTimer(TankSpawnTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnInterval, true, SpawnInterval);
}

void ATankFactory::TakeDamage(const FDamageData& DamageData)
{
	if (bIsFactoryAlive)
	{
		HealthComponent->TakeDamage(DamageData);
	}
}

void ATankFactory::SpawnNewTank()
{	
	if (bIsFactoryAlive)
	{
		FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1.f));
		ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		NewTank->SetMovingPoints(WayPoints);
		NewTank->FinishSpawning(SpawnTransform);
	}
}

void ATankFactory::OnDie()
{
	GetWorld()->GetTimerManager().ClearTimer(TankSpawnTimerHandle);
	bIsFactoryAlive = false;
	BuildingMesh->SetHiddenInGame(true);
	DestroyedBuildingMesh->SetHiddenInGame(false);
	//Destroy();
}

void ATankFactory::OnHealthChanged(float Amount)
{
	UE_LOG(LogTemp, Warning, TEXT("%s taked damage: %f"), *GetName(), Amount);
}

