// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClasses/BasePawn.h"
#include <Components/ArrowComponent.h>
#include "Components/HealthComponent.h"
#include <Components/BoxComponent.h>
#include "Cannons/Cannon.h"
#include <Components/StaticMeshComponent.h>
#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include <Sound/SoundBase.h>

#include "Components/WidgetComponent.h"
#include "UI/HealthWidget.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
	SetRootComponent(BaseMeshComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMeshComponent->SetupAttachment(BaseMeshComponent);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSpawn"));
	CannonSpawnPoint->SetupAttachment(TurretMeshComponent);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	HitCollider->SetupAttachment(RootComponent);

	DyingVisibleEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("ShootVisibleEffect"));
	//DyingVisibleEffect->AddToRoot();

	DyingAudioEffect = CreateDefaultSubobject<USoundBase>(TEXT("ShootAudioEffect"));
//	DyingAudioEffect->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddDynamic(this, &ABasePawn::OnDie);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ABasePawn::OnHealthChanged);

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon(DefaultCannonClass, MaxAmmo);
	CurrentCannonIndex = 0;
}


// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UHealthWidget* HealthWidget = Cast<UHealthWidget>(HealthWidgetComponent->GetUserWidgetObject()))
	{
		UE_LOG(LogTemp, Warning, TEXT("HEALTH OK"));
		HealthWidget->SetHealthPercent(GetHealthPercent());
	}
}

void ABasePawn::OnHealthChanged_Implementation(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("%s taked damage: %f"), *GetName(), DamageAmount);
}

void ABasePawn::OnDie_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DyingVisibleEffect, GetActorLocation(), GetActorRotation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DyingAudioEffect, GetActorLocation(), GetActorRotation());

	if (Cannon)
	{
		Cannon->Destroy();
	}
	Destroy();
}

float ABasePawn::GetHealthPercent() const
{
	return HealthComponent->GetHealthState();
}

void ABasePawn::Fire()
{
	if (!Cannon) return;
		
	Cannon->Fire();
}

void ABasePawn::SetupCannon(TSubclassOf<class ACannon> InCannonClass, int32 AmmoAmount)
{
	if (InCannonClass && Cannons.Num() < MaxCannons)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor <ACannon>(InCannonClass, Params);
		if (!Cannon) return;

		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Cannon->AddAmmo(AmmoAmount);
		Cannons.Add(Cannon);
		CurrentCannonIndex = (CurrentCannonIndex + 1) % MaxCannons;
	}
	else
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor <ACannon>(InCannonClass, Params);
		if (!Cannon) return;

		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Cannon->AddAmmo(AmmoAmount);
		Cannons[CurrentCannonIndex] = Cannon;
	}
}


