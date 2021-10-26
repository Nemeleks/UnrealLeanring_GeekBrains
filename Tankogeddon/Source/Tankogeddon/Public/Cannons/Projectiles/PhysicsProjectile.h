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


protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPhysicsMovementComponent* MovementComponent;

};
