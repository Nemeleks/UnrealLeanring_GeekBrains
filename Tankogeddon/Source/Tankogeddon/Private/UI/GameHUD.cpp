// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameHUD.h"
#include "Blueprint/UserWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	auto PlayerHUDWidget = CreateWidget < UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}
}
