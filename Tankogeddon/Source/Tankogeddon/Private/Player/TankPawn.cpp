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
	//SetupCannon(DefaultCannonClass);

	Cannon->ScoreOnKill.AddDynamic(this, &ATankPawn::AddScoreForKill);
}


void ATankPawn::AddScoreForKill(float Amount)
{
	CurrentScore += Amount;
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

//void ATankPawn::Fire()
//{
//	Super::Fire();
//}

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

	Cannons[CurrentCannonIndex]->SetActorHiddenInGame(true);

	CurrentCannonIndex = (CurrentCannonIndex + 1) % Cannons.Num();
	Cannons[CurrentCannonIndex]->SetActorHiddenInGame(false);

	Cannon = Cannons[CurrentCannonIndex];
}

