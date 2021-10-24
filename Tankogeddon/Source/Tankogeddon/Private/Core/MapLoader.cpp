// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MapLoader.h"
#include <Components/BoxComponent.h>
#include <Components/PointLightComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AMapLoader::AMapLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);

	MapLoaderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MapLoaderMesh"));
	MapLoaderMesh->SetupAttachment(RootComponent);

	OverlapCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapCollider"));
	OverlapCollider->SetupAttachment(RootComponent);

	ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("ActivatedLight"));
	ActivatedLight->SetupAttachment(RootComponent);

	DeativatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("DeativatedLight"));
	DeativatedLight->SetupAttachment(RootComponent);

	OverlapCollider->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnMeshOverlapBegin);
}

void AMapLoader::SetIsActicated(bool NewIsActivated)
{
	bIsActivated = NewIsActivated;
	SetActiveLights();
}

// Called when the game starts or when spawned
void AMapLoader::BeginPlay()
{
	Super::BeginPlay();
	SetActiveLights();
}

void AMapLoader::SetActiveLights()
{
	ActivatedLight->SetHiddenInGame(!bIsActivated);
	DeativatedLight->SetHiddenInGame(bIsActivated);
}

void AMapLoader::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActivated) return;

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (OtherActor == PlayerPawn)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
	}
}


