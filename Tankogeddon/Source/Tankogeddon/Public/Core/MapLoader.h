// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapLoader.generated.h"

UCLASS()
class TANKOGEDDON_API AMapLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapLoader();

	UFUNCTION(BlueprintCallable, Category = "MapLoader")
	void SetIsActicated(bool NewIsActivated);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* MapLoaderMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* OverlapCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPointLightComponent* ActivatedLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPointLightComponent* DeativatedLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Loading Params")
	FName LoadLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Loading Params")
	bool bIsActivated = false;

private:
	void SetActiveLights();

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
