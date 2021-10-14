// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceClasses/Damageable.h"
#include "InterfaceClasses/Scorable.h"
#include "Turret.generated.h"

UCLASS()
class TANKOGEDDON_API ATurret : public AActor, public IDamageable, public IScorable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	TSubclassOf<class ACannon> CannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cannon")
	int32 StartAmmo = 30;

	UPROPERTY()
	class ACannon* Cannon;

	UPROPERTY()
	class APawn* PlayerPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 2000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accuracy = 10.f;

	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
	void Fire();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnHealthChanged(float DamageAmount);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float ScoreForKill = 10;

public:
	void TakeDamage(const FDamageData& DamageData) override;
	float GetScoreForKill() override { return ScoreForKill; };
};
