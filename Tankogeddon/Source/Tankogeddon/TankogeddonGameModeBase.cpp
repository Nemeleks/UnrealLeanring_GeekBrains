// Copyright Epic Games, Inc. All Rights Reserved.


#include "TankogeddonGameModeBase.h"
#include "InterfaceClasses/Scorable.h"
#include "Player/TankPlayerController.h"

void ATankogeddonGameModeBase::NotifyActorDestroyedByDamage(AActor* Actor, const FDamageData& DamageData)
{

	if (IScorable* Scorable = Cast<IScorable>(Actor))
	{
		ATankPlayerController* TankPlayerController = Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController());
		if (DamageData.Instigator == TankPlayerController->GetPawn())
		{
			TankPlayerController->AddScore(Scorable->GetScoreForKill());
		}		
	}	
}
