// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NewGameBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);

	if (OptionsBtn)
	{
		OptionsBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);
	}

	QuitGameBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitGameClicked);
}

void UMainMenuWidget::OnNewGameClicked()
{
	UGameplayStatics::OpenLevel(this, FName("Test"));
	RemoveFromParent();
}

void UMainMenuWidget::OnOptionsClicked()
{
}

void UMainMenuWidget::OnQuitGameClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit, true);
}
