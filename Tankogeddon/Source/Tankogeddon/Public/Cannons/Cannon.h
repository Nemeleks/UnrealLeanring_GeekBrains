// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs/GameStructs.h"
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float Damage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	int32 MaxAmmo = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	ECannonType CannonType = ECannonType::FireProjectiles;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	int32 MultiplyFireMaxCount = 3;

private:
	bool bIsReadyToFireProjectiles = false;
	bool bIsReadyToFireTrace = false;
	bool bIsReadyToMultiplyFire = false;

	FTimerHandle ProjectilesReloadTimerHandle;
	FTimerHandle TraceReloadTimerHandle;
	FTimerHandle TraceFireTimerHandle;

	int32 CurrentAmmo = 0;
	int32 MultiplyFireCurrent = 0;
public:	
	// Sets default values for this actor's properties
	ACannon();

	void Fire();
	void ProjectilesFire();
	void TraceFire();

	int32 GetCurrentAmmo() const { return CurrentAmmo; };
	ECannonType GetCannonType() const { return CannonType; };
	FVector GetCannonLocation() const { return GetActorLocation(); };

	void SetCannonType(ECannonType Type) { CannonType = Type; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	void ProjectilesReload();
	void TraceReload();
};
