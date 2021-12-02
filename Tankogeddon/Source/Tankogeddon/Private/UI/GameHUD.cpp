// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/MainMenuGameMode.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Tankogeddon/TankogeddonGameModeBase.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (PlayerController)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, nullptr, EMouseLockMode::DoNotLock, false);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	if (Cast<AMainMenuGameMode>(GetWorld()->GetAuthGameMode()))
	{
		UseWidget(EWidgetID::MainMenu);
	}
	else if (Cast<ATankogeddonGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		UseWidget(EWidgetID::PlayerHUD);
	}
}

UUserWidget* AGameHUD::UseWidget(EWidgetID WidgetID, bool RemovePrevious, int32 ZOrder)
{
	UE_LOG(LogTemp, Warning, TEXT("Try UseWidget"));
	UE_LOG(LogTemp, Warning, TEXT("WidgetID = %d"), WidgetID);
	UE_LOG(LogTemp, Warning, TEXT("CurrentWidgetID = %d"), CurrentWidgetID);
	if (CurrentWidgetID == WidgetID)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentWidgetID == WidgetID"));
		return CurrentWidget;
	}

	if (RemovePrevious)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemovePrevious"));
		RemoveCurrentWidgetFromViewport();
	}

	TSubclassOf<UUserWidget> WidgetClassToUse = WidgetClasses.FindRef(WidgetID);
	if (WidgetClassToUse.Get())
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetClassToUse.Get() OK"));
		CurrentWidgetID = WidgetID;
		return AddWidgetByClass(WidgetClassToUse, ZOrder);
	}

	return nullptr;
}

UUserWidget* AGameHUD::GetCurrentWidget()
{
	return CurrentWidget;
}

void AGameHUD::RemoveCurrentWidgetFromViewport()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;

		CurrentWidgetID = EWidgetID::None;
	}
}

UUserWidget* AGameHUD::AddWidgetByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder)
{
    UE_LOG(LogTemp, Warning, TEXT("AddWidgetByClass"));
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
    if (!CurrentWidget)
    {
    	UE_LOG(LogTemp, Warning, TEXT("Nullptr CurrentWidget"));
    }	
	if (CurrentWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentWidget Add To Viewport !"));
		CurrentWidget->AddToViewport(ZOrder);
	}

	return CurrentWidget;
}
