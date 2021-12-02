// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/MainMenuGameMode.h"

#include "UI/GameHUD.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	HUDClass = AGameHUD::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	// UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay"));
	// if (HUDClass)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("HUD Class OK"));
	// 	AGameHUD* HUD = Cast<AGameHUD>(HUDClass->referenc);
	// 	if (HUD)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("HUD Class CAST TRUE"));
	// 		HUD->UseWidget(EWidgetID::MainMenu, false);
	// 	}
	// }
}
