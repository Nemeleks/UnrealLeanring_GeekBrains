// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannons/Projectiles/BaseProjectile.h"
#include "PhysicsProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API APhysicsProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	APhysicsProjectile();

	virtual void Start() override;
	virtual void Stop() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float ExpolionRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float ExpolionImpulse = 100.f;
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPhysicsMovementComponent* MovementComponent;

	void OnComponentHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
