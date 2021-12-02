// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
public:
	virtual void NativeConstruct() override;
	void SetHealthPercent(float Health) {HealthPercent = Health;}

	UFUNCTION()
	float GetHP() {return HealthPercent;}

private:
	float HealthPercent = 0;
	
};
