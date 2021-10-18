// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannons/Cannon.h"
#include <Components/ArrowComponent.h>
#include "DrawDebugHelpers.h"
#include "Player/TankPawn.h"
#include "Cannons/Projectiles/BaseProjectile.h"
#include "Subsystems/ActolPoolSubsystem.h"
#include "InterfaceClasses/Damageable.h"
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ForceFeedbackEffect.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComponent = CreateDefaultSubobject < USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPount"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

	ShootVisibleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootVisibleEffect"));
	ShootVisibleEffect->SetupAttachment(ProjectileSpawnPoint);

	ShootAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootAudioEffect"));
	ShootAudioEffect->SetupAttachment(ProjectileSpawnPoint);
}

void ACannon::Fire()
{
	if (CurrentAmmo < 1) return;


	if (CannonType == ECannonType::ProjectileCannon)
	{
		if (!bIsReadyToFireProjectiles) return;
		CurrentAmmo -= 1;
		bIsReadyToFireProjectiles = false;
		ShootVisibleEffect->ActivateSystem();
		ShootAudioEffect->Play();

		if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			if (ShootForceEffect)
			{
				FForceFeedbackParameters Params;
				Params.bLooping = false;
				Params.Tag = TEXT("ShootFFParams");
				GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect);
			}

		}
		ProjectilesFire();
		GetWorld()->GetTimerManager().SetTimer(ProjectilesReloadTimerHandle, this, &ACannon::ProjectilesReload, 1.f / FireRate, false);
	}
	else if (CannonType == ECannonType::TraceCannon)
	{
		if (!bIsReadyToFireTrace) return;
		CurrentAmmo -= 1;
		bIsReadyToFireTrace = false;
		ShootVisibleEffect->ActivateSystem();
		ShootAudioEffect->Play();
		TraceFire();
		GetWorld()->GetTimerManager().SetTimer(TraceReloadTimerHandle, this, &ACannon::TraceReload, 2.f / FireRate, false);
	}
	else if (CannonType == ECannonType::Machinegun)
	{
		if (!bIsReadyToMachinegunFire) return;
		CurrentAmmo -= 1;
		bIsReadyToMachinegunFire = false;
		ProjectilesFire();
		GetWorld()->GetTimerManager().SetTimer(MachinegunReloadTimerHandle, this, &ACannon::MachinegunReload, 1.f / FireRateMachinegun, false);
	}
}
	

void ACannon::ProjectilesFire()
{
	//ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Fire - Projectile"));
	UActolPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActolPoolSubsystem>();
	FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
	ABaseProjectile* Projectile = Cast<ABaseProjectile>(Pool->MoveActorFromPool(ProjectileClass, SpawnTransform));
	if (Projectile)
	{
		Projectile->GetScoreOnKill.AddDynamic(this, &ACannon::GetScoreOnKill);
		Projectile->SetInstigator(GetInstigator());
		Projectile-> Start();
	}
}

void ACannon::TraceFire()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Purple, TEXT("Fire Trace"));

	FHitResult HitResult;
	FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
	FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * TraceFireRange;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Red, false, 0.5f, 0, 5.f);
		if (HitResult.Actor.IsValid() && HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
		{
			HitResult.Actor->Destroy();
		}
		else if (IDamageable* Damageable = Cast<IDamageable>(HitResult.Actor))
		{
			FDamageData DamageData;
			DamageData.DamageAmount = Damage;
			DamageData.Instigator = GetInstigator();
			DamageData.DamageMaker = this;
			Damageable->TakeDamage(DamageData);
			//UE_LOG(LogTemp, Warning, TEXT("DamageableActorName: "), *HitResult.Actor->GetName());

			if (IScorable* Scorable = Cast<IScorable>(HitResult.Actor))
			{
				if (HitResult.Actor->IsActorBeingDestroyed())
				{
					if (ScoreOnKill.IsBound())
					{
						ScoreOnKill.Broadcast(Scorable->GetScoreForKill());
					}
				}
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
	}
}

void ACannon::MultiplyFire()
{

	MultiplyFireCurrent += 1;
	if (MultiplyFireCurrent == MultiplyFireMaxCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(MultiplyFireTimerHandle);
	}
	ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	ShootVisibleEffect->ActivateSystem();
	ShootAudioEffect->Play();
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Fire - Projectile"));
	if (Projectile)
	{
		Projectile->GetScoreOnKill.AddDynamic(this, &ACannon::GetScoreOnKill);
		Projectile->SetInstigator(GetInstigator());
		Projectile->Start();
	}


}

void ACannon::AltFire()
{
	if (CurrentAmmo < 1) return;
	if (CannonType == ECannonType::ProjectileCannon)
	{
		if (!bIsReadyToFireProjectiles) return;
		CurrentAmmo -= 1;
		bIsReadyToFireProjectiles = false;
		if (bIsReadyToMultiplyFire)
		{
			bIsReadyToMultiplyFire = false;
			GetWorld()->GetTimerManager().SetTimer(MultiplyFireTimerHandle, this, &ACannon::MultiplyFire, 0.3f / FireRate, true);
		}
		GetWorld()->GetTimerManager().SetTimer(MultiplyFireReloadTimerHandle, this, &ACannon::MultiplyFireReload, 3.f / FireRate, false);
	}
	else if (CannonType == ECannonType::TraceCannon)
	{

	}
	else if (CannonType == ECannonType::Machinegun)
	{
		if (!bIsReadyToIncreaseSpeedMachinegun) return;
		bIsReadyToIncreaseSpeedMachinegun = false;
		FireRateMachinegun /= 2;
		GetWorld()->GetTimerManager().SetTimer(MachinegunIncreaseSpeedTimerHandle, this, &ACannon::MachinegunIncreaseSpeed, 3.f, false);
	}
	
}

void ACannon::GetScoreOnKill(float Amount)
{
	ScoreOnKill.Broadcast(Amount);
}

void ACannon::AddAmmo(int32 Amount)
{
	CurrentAmmo += Amount;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bIsReadyToFireProjectiles = true;
	bIsReadyToFireTrace = true;
	bIsReadyToMultiplyFire = true;
	bIsReadyToMachinegunFire = true;
	bIsReadyToIncreaseSpeedMachinegun = true;

}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ACannon::ProjectilesReload()
{
	bIsReadyToFireProjectiles = true;
}

void ACannon::MultiplyFireReload()
{
	bIsReadyToFireProjectiles = true;
	bIsReadyToMultiplyFire = true;
	MultiplyFireCurrent = 0;
}

void ACannon::TraceReload()
{
	bIsReadyToFireTrace = true;
}

void ACannon::MachinegunReload()
{
	bIsReadyToMachinegunFire = true;
}

void ACannon::MachinegunIncreaseSpeed()
{
	FireRateMachinegun *= 2;
	GetWorld()->GetTimerManager().SetTimer(MachinegunIncreaseSpeedReloadTimerHandle, this, &ACannon::MachinegunIncreaseSpeed, 3.f, false);
}

void ACannon::MachinegunIncreaseSpeedReload()
{
	bIsReadyToIncreaseSpeedMachinegun = true;
}
