// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TankPlayerController.h"
#include "Player/TankPawn.h"
#include "DrawDebugHelpers.h"
#include "Cannons/Cannon.h"
#include "Subsystems/ActolPoolSubsystem.h"

ATankPlayerController::ATankPlayerController()
{
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("TurnTank", this, &ATankPlayerController::TurnTank);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("AltFire", IE_Pressed, this, &ATankPlayerController::AltFire);
	InputComponent->BindAction("ChangeCannon", IE_Pressed, this, &ATankPlayerController::ChangeCannon);
	InputComponent->BindAxis("LiftCannon", this, &ATankPlayerController::LiftCannon);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	const auto Cannon = TankPawn->GetCannon();
	if (!Cannon) return;
	FVector CannonCurrentLocation = Cannon->GetCannonLocation();

	FVector WorldMouseDirection;
	DeprojectMousePositionToWorld(MousePos, WorldMouseDirection);
	//FVector MousePosition = MousePos;
	MousePos.Z = TankPawn->GetTurretMeshLocation().Z;
	//MousePosition.Z = TankPawn->GetActorLocation().Z;
	
	FVector TurretTargetDirection = MousePos - TankPawn->GetTurretMeshLocation();

	TurretTargetDirection.Normalize();

	//FVector TurretTargetDirection = MousePosition - TankPawn->GetActorLocation();
	//TurretTargetDirection.Normalize();
	//UE_LOG(LogTemp, Warning, TEXT("Mouse X = % f, Y = % f, Z = % f"), MousePos.X, MousePos.Y, MousePos.Z);
	//UE_LOG(LogTemp, Warning, TEXT("Loc X = % f, Y = % f, Z = % f"), TankPawn->GetTurretMeshLocation().X, TankPawn->GetTurretMeshLocation().Y, TankPawn->GetTurretMeshLocation().Z);
	//UE_LOG(LogTemp, Warning, TEXT("Dir X = % f, Y = % f, Z = % f"), TurretTargetDirection1.X, TurretTargetDirection1.Y, TurretTargetDirection1.Z);
	//UE_LOG(LogTemp, Error, TEXT("Mouse X = % f, Y = % f, Z = % f"), MousePosition.X, MousePosition.Y, MousePosition.Z);
	//UE_LOG(LogTemp, Error, TEXT("Loc X = % f, Y = % f, Z = % f"), TankPawn->GetActorLocation().X, TankPawn->GetActorLocation().Y, TankPawn->GetActorLocation().Z);	
	//UE_LOG(LogTemp, Error, TEXT("Dir X = % f, Y = % f, Z = % f"), TurretTargetDirection.X, TurretTargetDirection.Y, TurretTargetDirection.Z);
	

	FVector TurretTargetPosition = TankPawn->GetTurretMeshLocation() + TurretTargetDirection * 1000.f;

	DrawDebugLine(GetWorld(), TankPawn->GetTurretMeshLocation(), TurretTargetPosition, FColor::Green, false, 0.1f, 0, 5.f);
	TankPawn->SetTurretTargetPosition(TurretTargetPosition);
	
	

	float angle = FMath::DegreesToRadians(90.f + TankPawn->GetCannon()->GetCannonPitchRotation()); 
	float cos = FMath::Cos(angle);
	float sin = FMath::Sin(angle);
	float speed = TankPawn->GetCannon()->GetProjectileMovementSpeed();

	float initial_height = TankPawn->GetCannon()->GetProgectileSpawnPointLocation().Z;
	float gravity = -GetWorld()->GetGravityZ();

	UE_LOG(LogTemp, Display, TEXT("ProjectileSpeed = %f"), TankPawn->GetCannon()->GetProjectileMovementSpeed());
	UE_LOG(LogTemp, Warning, TEXT("Cannon Pitch = %f"), 90.f + TankPawn->GetCannon()->GetCannonPitchRotation());

	float range = (speed * cos / gravity) * (speed * sin + FMath::Sqrt(speed * speed * sin * sin + 2 * gravity * initial_height));

	FVector EndPosit = TankPawn->GetCannon()->GetProgectileSpawnPointLocation() + TurretTargetDirection * range;
	EndPosit.Z = 0.f;

	DrawDebugSphere(GetWorld(), EndPosit, 20.f, 30, FColor::Red, false, 0.1f, 0, 5.f);

}

void ATankPlayerController::AddScore(float ScoreToAdd)
{
	CurrentScore += ScoreToAdd;
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

void ATankPlayerController::LiftCannon(float Amount)
{
	TankPawn->LiftCannon(Amount);
}

void ATankPlayerController::Fire()
{
	TankPawn->Fire();
}

void ATankPlayerController::AltFire()
{
	TankPawn->AltFire();
}

void ATankPlayerController::ChangeCannon()
{
	TankPawn->ChangeCannon();
}

void ATankPlayerController::DumpActorPoolStats()
{
	GetWorld()->GetSubsystem<UActolPoolSubsystem>()->DumpPoolStats();
}

