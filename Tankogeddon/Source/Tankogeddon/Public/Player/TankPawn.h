// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseClasses/BasePawn.h"
#include "InterfaceClasses/Damageable.h"
#include "InterfaceClasses/Scorable.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public ABasePawn, public IDamageable, public IScorable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();


	void TakeDamage(const FDamageData& DamageData) override;


	float GetScoreForKill() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Targeting")
	float RotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Smoothness")
	float MoveSmoothness = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Smoothness")
	float RotationSmoothness = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float TurretRotationSmoothness = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params", meta = (MakeEditWidget = true))
	TArray<FVector> MovePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params")
	float MovementAccuracy = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scoring")
	float ScoreForKill = 20.f;

private:
	float TargetForwardAxisValue = 0.f;
	float TargetTurnAxisValue = 0.f;

	float CurrentForwardAxisValue = 0.f;
	float CurrentRightAxisValue = 0.f;

	float CurrentScore = 0.f;

	FVector TurretTargetPosition;


	TSubclassOf<class ACannon> CurrentCannon;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void TurnTank(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTurretTargetPosition(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	FVector GetTurretMeshLocation() const { return TurretMeshComponent->GetComponentLocation(); };

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void AltFire();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void ChangeCannon();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	ACannon* GetCannon() const {return Cannon;}

	UFUNCTION(BlueprintCallable, Category = "Turret")
	TArray<ACannon*> GetCannons() const {return Cannons;}

	UFUNCTION(BlueprintPure, Category = "Turret")
	FVector GetTurretForwardVector();

	UFUNCTION(BlueprintCallable, Category = "Scoring")
	void AddScoreForKill(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Scoring")
	float GetCurrentScore() const { return CurrentScore; }

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	const TArray<FVector>& GetMovePoints() {return MovePoints;}

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	float GetMovementAccuracy() {return MovementAccuracy;}

};
