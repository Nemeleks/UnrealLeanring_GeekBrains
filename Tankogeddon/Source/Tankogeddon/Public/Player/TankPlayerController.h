// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class ATankPawn* TankPawn;
	UPROPERTY()
	FVector MousePos;

public:
	ATankPlayerController();
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	FVector GetMousePos() const { return MousePos; };

protected:
	virtual void BeginPlay() override;

private:
	void MoveForward(float Amount);

	void TurnTank(float Amount);

	void Fire();

	void ChangeCannon();
};
