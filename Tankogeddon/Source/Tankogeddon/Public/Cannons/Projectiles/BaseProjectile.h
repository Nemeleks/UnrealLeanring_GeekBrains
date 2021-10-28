// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetScoreOnKill, float, Amount);

UCLASS()
class TANKOGEDDON_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float FireRange = 10000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float Damage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scoring")
	float Score = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
	float Mass = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics|Explosion")
	bool bIsExlosiveProjectile = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics|Explosion", meta = (EditCondition = "bIsExlosiveProjectile"))
		float ExpolionRange = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics|Explosion", meta = (EditCondition = "bIsExlosiveProjectile"))
		float ExpolionImpulse = 100.f;

public:
	// Sets default values for this actor's properties
	ABaseProjectile();

	virtual void Start();
	virtual void Stop();

	UFUNCTION()
	float GetMoveSpeed() const { return MoveSpeed; }

	UPROPERTY()
	FGetScoreOnKill GetScoreOnKill;

protected:
	UFUNCTION()
	virtual void OnComponentHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& HitResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector StartPosition;
};
