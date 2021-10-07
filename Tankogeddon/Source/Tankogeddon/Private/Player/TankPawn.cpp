// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TankPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tankogeddon/Tankogeddon.h"
#include "Cannons/Cannon.h"
#include <Components/ArrowComponent.h>

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



}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon();
}

void ATankPawn::SetupCannon()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}

	FActorSpawnParameters Params;
	Params.Instigator = this;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor <ACannon>(DefaultCannonClass, Params);
	Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentForwardAxisValue = FMath::Lerp(CurrentForwardAxisValue, TargetForwardAxisValue, MoveSmoothness * DeltaTime);
	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector NewLocation = CurrentLocation + ForwardVector * CurrentForwardAxisValue * MoveSpeed * DeltaTime;
	

	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, TargetTurnAxisValue, RotationSmoothness * DeltaTime);
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
	TurretMeshComponent->SetWorldRotation(FMath::RInterpTo(TurrentCurrentRotation, TurretTargetRotation, DeltaTime, TurretRotationSmoothness));
	//TurretMeshComponent->SetWorldRotation(TurretTargetRotation);
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

void ATankPawn::ChangeCannon()
{
	if (!Cannon) return;

	if (Cannon->GetCannonType() == ECannonType::FireProjectiles)
	{
		Cannon->SetCannonType(ECannonType::FireTrace);
	}
	else if (Cannon->GetCannonType() == ECannonType::FireTrace)
	{
		Cannon->SetCannonType(ECannonType::FireProjectiles);
	}
}

