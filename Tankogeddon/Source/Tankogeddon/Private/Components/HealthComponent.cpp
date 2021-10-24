// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "../TankogeddonGameModeBase.h"
#include "Cannons/AmmoBoxes/BaseAmmoBox.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UHealthComponent::TakeDamage(const FDamageData& DamageData)
{
	float TakedDamageAmount = DamageData.DamageAmount;
	CurrentHealth -= TakedDamageAmount;

	if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (ShootShakeEffect)
		{
			FForceFeedbackParameters Params;
			Params.bLooping = false;
			Params.Tag = TEXT("ShootFFParams");
			GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ShootShakeEffect);
		}
	}

	if (CurrentHealth <= 0.f)
	{	
		if (OnDie.IsBound())
		{
			OnDie.Broadcast();
		}

		const auto GameMode = GetWorld()->GetAuthGameMode();
		ATankogeddonGameModeBase* TankogeddonGameMode = Cast<ATankogeddonGameModeBase>(GameMode);

		TankogeddonGameMode->NotifyActorDestroyedByDamage(GetOwner(), DamageData);

		if (LootList.Num() >0)
		{
			int32 LootIndex = FMath::RandRange(0, LootList.Num() - 1);
			if (GetOwner() != GetWorld()->GetFirstPlayerController())
			{
				GetWorld()->SpawnActor<ABaseAmmoBox>(LootList[LootIndex], GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
			}
		}
	}
	else
	{
		if (OnHealthChanged.IsBound())
		{
			OnHealthChanged.Broadcast(TakedDamageAmount);
		}
	}
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::AddHealth(float AddHealthAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + AddHealthAmount, 0.f, MaxHealth);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
}



