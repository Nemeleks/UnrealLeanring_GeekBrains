// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* TankMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* TurretMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* CannonSpawnPoint;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<class ACannon> DefaultCannonClass;

private:
	float TargetForwardAxisValue = 0.f;
	float TargetTurnAxisValue = 0.f;

	float CurrentForwardAxisValue = 0.f;
	float CurrentRightAxisValue = 0.f;

	FVector TurretTargetPosition;

	UPROPERTY()
	class ACannon* Cannon = nullptr;

	void SetupCannon();

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
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void ChangeCannon();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	ACannon* GetCannon() const {return Cannon;};

	//FVector GetTurretPosition() const { return TurretTargetPosition; };
};
