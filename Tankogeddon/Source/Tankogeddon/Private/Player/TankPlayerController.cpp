// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TankPlayerController.h"
#include "Player/TankPawn.h"
#include "DrawDebugHelpers.h"
#include "Cannons/Cannon.h"

ATankPlayerController::ATankPlayerController()
{
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("TurnTank", this, &ATankPlayerController::TurnTank);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("ChangeCannon", IE_Pressed, this, &ATankPlayerController::ChangeCannon);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	const auto Cannon = TankPawn->GetCannon();
	if (!Cannon) return;
	FVector CannonCurrentLocation = Cannon->GetCannonLocation();

	FVector WorldMouseDirection;
	DeprojectMousePositionToWorld(MousePos, WorldMouseDirection);
	MousePos.Z = TankPawn->GetActorLocation().Z;

	FVector TurretTargetDirection = MousePos - TankPawn->GetActorLocation();
	TurretTargetDirection.Normalize();
	FVector TurretTargetPosition = CannonCurrentLocation + TurretTargetDirection * 1000.f;
	DrawDebugLine(GetWorld(), CannonCurrentLocation, TurretTargetPosition, FColor::Green, false, 0.1f, 0, 5.f);
	TankPawn->SetTurretTargetPosition(TurretTargetPosition);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	bShowMouseCursor = true;
}

void ATankPlayerController::MoveForward(float Amount)
{
	TankPawn->MoveForward(Amount);
}

void ATankPlayerController::TurnTank(float Amount)
{
	TankPawn->TurnTank(Amount);
}

void ATankPlayerController::Fire()
{
	TankPawn->Fire();
}

void ATankPlayerController::ChangeCannon()
{
	TankPawn->ChangeCannon();
}

