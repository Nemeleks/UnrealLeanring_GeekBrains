// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs/GameStructs.h"
#include "Cannon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreOnKill, float, Amount);

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* ShootVisibleEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* ShootAudioEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float Damage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	ECannonType CannonType = ECannonType::ProjectileCannon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	int32 MultiplyFireMaxCount = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float ProjectileFireRange = 7000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float TraceFireRange = 5000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRateMachinegun = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	class UForceFeedbackEffect* ShootForceEffect;

private:
	bool bIsReadyToFireProjectiles = false;
	bool bIsReadyToMultiplyFire = false;

	bool bIsReadyToFireTrace = false;

	bool bIsReadyToMachinegunFire = false;
	bool bIsReadyToIncreaseSpeedMachinegun = false;

	FTimerHandle ProjectilesReloadTimerHandle;
	FTimerHandle MultiplyFireReloadTimerHandle;
	FTimerHandle MultiplyFireTimerHandle;

	FTimerHandle TraceReloadTimerHandle;

	FTimerHandle MachinegunReloadTimerHandle;
	FTimerHandle MachinegunIncreaseSpeedTimerHandle;
	FTimerHandle MachinegunIncreaseSpeedReloadTimerHandle;

	int32 CurrentAmmo = 0;
	int32 MultiplyFireCurrent = 0;
public:	
	// Sets default values for this actor's properties
	ACannon();

	UPROPERTY()
	FScoreOnKill ScoreOnKill;
	
	void Fire();
	void ProjectilesFire();
	void TraceFire();
	void MultiplyFire();
	void AltFire();

	FVector GetProgectileSpawnPointLocation();
	float GetCannonPitchRotation() const;

	UFUNCTION()
	int32 GetCurrentAmmo() const { return CurrentAmmo; };

	UFUNCTION()
	void GetScoreOnKill(float Amount);

	UFUNCTION()
	void AddAmmo(int32 Amount);
	
	UFUNCTION()
	ECannonType GetCannonType() const { return CannonType; };

	UFUNCTION()
	FVector GetCannonLocation() const { return GetActorLocation(); };

	void SetCannonType(ECannonType Type) { CannonType = Type; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	void ProjectilesReload();
	void MultiplyFireReload();
	void TraceReload();
	void MachinegunReload();
	void MachinegunIncreaseSpeed();
	void MachinegunIncreaseSpeedReload();
};
