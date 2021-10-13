// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TankPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tankogeddon/Tankogeddon.h"
#include "Cannons/Cannon.h"
#include <Components/ArrowComponent.h>
#include "Components/HealthComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	TankMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
	SetRootComponent(TankMeshComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMeshComponent->SetupAttachment(RootComponent);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSpawn"));
	CannonSpawnPoint->SetupAttachment(TurretMeshComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(TurretMeshComponent);
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->SocketOffset.X = -4500;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddDynamic(this, &ATankPawn::OnDie);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATankPawn::OnHealthChanged);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	//SetupCannon(DefaultCannonClass);
	SetupCannon(DefaultCannonClass, MaxAmmo);
	CurrentCannonIndex = 0;
}

void ATankPawn::OnHealthChanged_Implementation(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s taked damage: %f"), *GetName(), DamageAmount);
}

void ATankPawn::OnDie_Implementation()
{
	Destroy();
}

void ATankPawn::SetupCannon(TSubclassOf<class ACannon> InCannonClass, int32 AmmoAmount)
{
	if (InCannonClass && Cannons.Num() < MaxCannons)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor <ACannon>(InCannonClass, Params);
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Cannon->AddAmmo(AmmoAmount);
		Cannons.Add(Cannon);
		//UE_LOG(LogTemp, Warning, TEXT("CurrentCannonIndex"))
		CurrentCannonIndex = (CurrentCannonIndex + 1) % MaxCannons;
	}
	else
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		Cannon = GetWorld()->SpawnActor <ACannon>(InCannonClass, Params);
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Cannon->AddAmmo(AmmoAmount);
		Cannons[CurrentCannonIndex] = Cannon;
	}
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentForwardAxisValue = FMath::FInterpTo(CurrentForwardAxisValue, TargetForwardAxisValue, DeltaTime, MoveSmoothness);
	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector NewLocation = CurrentLocation + ForwardVector * CurrentForwardAxisValue * MoveSpeed * DeltaTime;
	

	CurrentRightAxisValue = FMath::FInterpTo(CurrentRightAxisValue, TargetTurnAxisValue, DeltaTime, RotationSmoothness);
	float YawRotation = RotationSpeed * CurrentRightAxisValue * DeltaTime;
	FRotator CurrentRotation = GetActorRotation();
	YawRotation = CurrentRotation.Yaw + YawRotation;
	FRotator NewRotation = FRotator(0, YawRotation, 0);

	//UE_LOG(LogTankogeddon, Warning, TEXT("CurrentForwardAxisValue = %f"), CurrentForwardAxisValue);
	SetActorLocationAndRotation(NewLocation, NewRotation, true);

	FRotator TurretTargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMeshComponent->GetComponentLocation(), TurretTargetPosition);
	FRotator TurrentCurrentRotation = TurretMeshComponent->GetComponentRotation();
	TurretTargetRotation.Pitch = TurrentCurrentRotation.Pitch;
	TurretTargetRotation.Roll = TurrentCurrentRotation.Roll;
	TurretMeshComponent->SetWorldRotation(FMath::RInterpConstantTo(TurrentCurrentRotation, TurretTargetRotation, DeltaTime, TurretRotationSmoothness));
}

void ATankPawn::MoveForward(float Amount)
{
	TargetForwardAxisValue = Amount;
}

void ATankPawn::TurnTank(float Amount)
{
	TargetTurnAxisValue = Amount;
}

void ATankPawn::SetTurretTargetPosition(const FVector& TargetPosition)
{
	TurretTargetPosition = TargetPosition;
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::AltFire()
{
	if (Cannon)
	{
		Cannon->AltFire();
	}
}

void ATankPawn::ChangeCannon()
{
	if (Cannons.Num() <= 1) return;

	Cannons[CurrentCannonIndex]->SetHidden(true);

	CurrentCannonIndex = (CurrentCannonIndex + 1) % Cannons.Num();
	Cannons[CurrentCannonIndex]->SetHidden(false);

	Cannon = Cannons[CurrentCannonIndex];
}

