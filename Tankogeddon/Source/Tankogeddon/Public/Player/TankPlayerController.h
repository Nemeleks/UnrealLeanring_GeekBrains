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

	UFUNCTION()
	void AddScore(float ScoreToAdd);

	UFUNCTION()
	float GetCurrentScore() const {return CurrentScore;}

protected:
	virtual void BeginPlay() override;

private:
	void MoveForward(float Amount);

	void TurnTank(float Amount);

	void Fire();

	void AltFire();

	void ChangeCannon();

	UFUNCTION(exec)
	void DumpActorPoolStats();

	float CurrentScore = 0;
};
