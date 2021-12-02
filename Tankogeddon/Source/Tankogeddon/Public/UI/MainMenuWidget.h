// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* NewGameBtn;
	
	UPROPERTY(meta = (BindWidgetOptional))
	class UButton* OptionsBtn; 
	
	UPROPERTY(meta = (BindWidget))
    class UButton* QuitGameBtn;

protected:
	UFUNCTION()
	void OnNewGameClicked();
	
	UFUNCTION()
	void OnOptionsClicked();
	
	UFUNCTION()
	void OnQuitGameClicked();
};
