// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStructs/GameStructs.h"
#include "UserInterface.generated.h"


/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UUserInterface : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetPlayerAmmo () const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetCannonTypeName() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetCurrentScore() const;
};
