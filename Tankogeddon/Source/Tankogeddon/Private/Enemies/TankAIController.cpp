// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/TankAIController.h"
#include "Player/TankPawn.h"
#include <DrawDebugHelpers.h>

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	if (TankPawn)
	{
		for (const FVector& Point : TankPawn->GetMovePoints())
		{
			MovePoints.Add(TankPawn->GetActorLocation() + Point);
		}
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPawn)
	{
		MoveToNextPoint();
		Targetring();
	}
}

void ATankAIController::MoveToNextPoint()
{
	if (MovePoints.Num() == 0)
	{
		return;
	}

	TankPawn->MoveForward(1.f);
	FVector PawnLocation = TankPawn->GetActorLocation();
	FVector CurrentPoint = MovePoints[CurrentPointIndex];
	if (FVector::DistSquared(PawnLocation, CurrentPoint) <= FMath::Square(TankPawn->GetMovementAccuracy()))
	{
		CurrentPointIndex++;
		if (!MovePoints.IsValidIndex(CurrentPointIndex))
		{
			CurrentPointIndex = 0;
		}
	}

	FVector MoveDirection = CurrentPoint - PawnLocation;
	MoveDirection.Normalize();
	FVector ForwardDirection = TankPawn->GetActorForwardVector();
	FVector RightDirection = TankPawn->GetActorRightVector();

	DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5.f);

	float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, MoveDirection)));
	float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, MoveDirection)));

	float RotationValue = 0.f;
	if (ForwardAngle > 5.f)
	{
		RotationValue = 1.f;
	}
	if (RightAngle > 90.f)
	{
		RotationValue = -RotationValue;
	}

	TankPawn->TurnTank(RotationValue);
}

void ATankAIController::Targetring()
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (FVector::DistSquared(PlayerPawn->GetActorLocation(), TankPawn->GetActorLocation()) > FMath::Square(TargetingRange))
	{
		return;
	}

	FHitResult HitResult;
	FVector TraceStart = TankPawn->GetActorLocation();
	FVector TraceEnd = PlayerPawn->GetActorLocation();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("AITrace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams) && HitResult.Actor != PlayerPawn)
	{
		return;
	}

	TankPawn->SetTurretTargetPosition(PlayerPawn->GetActorLocation());

	FVector TargetingDirection = TankPawn->GetTurretForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	DirectionToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer)));
	if (AimAngle <= Accuracy)
	{
		TankPawn->Fire();
	}
	
}

