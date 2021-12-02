// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthWidget.h"

#include "Components/ProgressBar.h"

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HealthBar->PercentDelegate.BindUFunction(this, FName("GetHP"));
	HealthBar->SynchronizeProperties();
}
