// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TANKOGEDDON_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BaseMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* TurretMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystem* DyingVisibleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USoundBase* DyingAudioEffect;

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnHealthChanged(float DamageAmount);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();

	UPROPERTY()
	class ACannon* Cannon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<class ACannon> DefaultCannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	int32 MaxAmmo = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	int32 MaxCannons = 2;

	UPROPERTY()
	int32 CurrentCannonIndex = 0;

	UPROPERTY()
	TArray<ACannon*> Cannons;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetupCannon(TSubclassOf<class ACannon> InCannonClass, int32 AmmoAmount);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const;

private:


};
