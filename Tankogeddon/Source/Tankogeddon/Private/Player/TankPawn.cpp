// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TankPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	TankMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
	//SetRootComponent(TankMeshComponent);
	RootComponent = TankMeshComponent;

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
	TurretMeshComponent->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritRoll = false;

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

	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector NewLocation = CurrentLocation + (ForwardVector * TargetForwardAxisValue + RightVector * TargetRightAxisValue) * MoveSpeed  * DeltaTime;

	SetActorLocation(NewLocation);
}

void ATankPawn::MoveForward(float Amount)
{
	TargetForwardAxisValue = Amount;
}

void ATankPawn::MoveRight(float Amount)
{
	TargetRightAxisValue = Amount;
}



