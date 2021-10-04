// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TankPlayerController.h"
#include "Player/TankPawn.h"

ATankPlayerController::ATankPlayerController()
{
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);
	InputComponent->BindAxis("TurnTank", this, &ATankPlayerController::TurnTank);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float Amount)
{
	TankPawn->MoveForward(Amount);
}

void ATankPlayerController::MoveRight(float Amount)
{
	TankPawn->MoveRight(Amount);
}

void ATankPlayerController::TurnTank(float Amount)
{
	TankPawn->TurnTank(Amount);
}

