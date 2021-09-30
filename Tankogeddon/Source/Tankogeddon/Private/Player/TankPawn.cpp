// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TankPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	TankMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TankMesh");
	SetRootComponent(TankMeshComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	TurretMeshComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector CurrentLocation = GetActorLocation();
	//FVector ForwardVector = GetActorForwardVector();
	//FVector RightVector = GetActorRightVector();
	//FVector NewLocation;
	//if (TargetRightAxisValue == 0)
	//{
	//	NewLocation = CurrentLocation + ForwardVector * MoveSpeed * TargetForwardAxisValue * DeltaTime;
	//}
	//else if (TargetForwardAxisValue == 0)
	//{
	//	NewLocation = CurrentLocation + RightVector * MoveSpeed * TargetRightAxisValue * DeltaTime;
	//}
	//else
	//{
	//	NewLocation = CurrentLocation;
	//}
	//SetActorLocation(NewLocation);
}

void ATankPawn::MoveForward(float Amount)
{
	//TargetForwardAxisValue = Amount;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ATankPawn::MoveRight(float Amount)
{
	//TargetRightAxisValue = Amount;
	AddMovementInput(GetActorRightVector(), Amount);
}



