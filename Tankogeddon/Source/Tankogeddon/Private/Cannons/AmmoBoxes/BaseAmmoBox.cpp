// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannons/AmmoBoxes/BaseAmmoBox.h"
#include "Player/TankPawn.h"
#include "Cannons/Cannon.h"
#include <Components/PrimitiveComponent.h>

// Sets default values
ABaseAmmoBox::ABaseAmmoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
}

void ABaseAmmoBox::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (OtherActor == PlayerPawn)
	{
		const auto Cannons = PlayerPawn->GetCannons();

		ACannon* Cannon = nullptr;
		for (auto i = 0; i < Cannons.Num(); ++i)
		{
			if (Cannons[i]->GetClass() == CannonClass)
			{
				Cannon = Cannons[i];
			}
		}
		if(Cannon)
		{
			Cannon->AddAmmo(AmmoToAdd);
		}
		else
		{
			PlayerPawn->SetupCannon(CannonClass, AmmoToAdd);
		}
		Destroy();
	}
}



