// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserInterface.h"
#include "Cannons/Cannon.h"
#include "Player/TankPawn.h"
#include "Player/TankPlayerController.h"

int32 UUserInterface::GetPlayerAmmo() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return 0;
	const auto PlayerTank = Cast<ATankPawn>(Player);
	const auto Cannon = PlayerTank->GetCannon();
	if (!Cannon) return	0;

	return Cannon->GetCurrentAmmo();
}

FString UUserInterface::GetCannonTypeName() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return "0";
	const auto PlayerTank = Cast<ATankPawn>(Player);
	const auto Cannon = PlayerTank->GetCannon();
	if (!Cannon) return	"0";
	FString CannonTypeName;

	if (Cannon->GetCannonType() == ECannonType::ProjectileCannon)
	{
		CannonTypeName = "Projectile Cannon";
	}
	else if (Cannon->GetCannonType() == ECannonType::TraceCannon)
	{
		CannonTypeName = "Trace Cannon";
	}
	return CannonTypeName;
}

float UUserInterface::GetCurrentScore() const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	const auto TankPlayerController = Cast<ATankPlayerController>(PlayerController);

	return TankPlayerController->GetCurrentScore();
}

float UUserInterface::GetPlayerHealthPercent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return 0;
	const auto PlayerTank = Cast<ATankPawn>(Player);
	return PlayerTank->GetHealthPercent();
}

