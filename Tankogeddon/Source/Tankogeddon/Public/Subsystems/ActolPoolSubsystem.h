// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActolPoolSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UActolPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TArray<AActor*> Actors;

	TMap<UClass*, TArray<AActor*>> InactiveActors;

	UFUNCTION()
	void OnActorDestroyed(AActor* Actor);

protected:
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

public:
	bool IsActorInPool(AActor* Actor) const;
	void MoveActorToPool(AActor* Actor);
	AActor* MoveActorFromPool(UClass* Class, const FTransform& Transform);

	void DumpPoolStats();
};
