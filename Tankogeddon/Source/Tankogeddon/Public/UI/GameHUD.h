// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */

UENUM()
enum class EWidgetID : uint8
{
	None,
	MainMenu,
	Options,
	GameOverScreen,
	PlayerHUD
};

UCLASS()
class TANKOGEDDON_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TMap<EWidgetID,TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	EWidgetID CurrentWidgetID = EWidgetID::None;

public:
	virtual void BeginPlay() override;

	UUserWidget* UseWidget(EWidgetID WidgetID, bool RemovePrevious = true, int32 ZOrder = 0);

	UUserWidget* GetCurrentWidget();

	void RemoveCurrentWidgetFromViewport();
	UUserWidget* AddWidgetByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder = 0);
	
};
