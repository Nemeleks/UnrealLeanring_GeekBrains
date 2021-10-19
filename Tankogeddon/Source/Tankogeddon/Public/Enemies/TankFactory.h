// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs/GameStructs.h"
#include <Engine/EngineTypes.h>
#include "InterfaceClasses/Damageable.h"
#include "TankFactory.generated.h"

UCLASS()
class TANKOGEDDON_API ATankFactory : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* DestroyedBuildingMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	class UArrowComponent* TankSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	class UBoxComponent* HitCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn params")
	float SpawnInterval = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
	TSubclassOf<class ATankPawn> SpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
	TArray<class ATargetPoint*> WayPoints;

public:	
	
	UFUNCTION()
	void TakeDamage(const FDamageData& DamageData);

protected:

	UFUNCTION()
	void SpawnNewTank();

	UFUNCTION()
	void OnDie();

	UFUNCTION()
	void OnHealthChanged(float Amount);

private:
	bool bIsFactoryAlive = false;

	FTimerHandle TankSpawnTimerHandle;
};
