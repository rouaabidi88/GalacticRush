// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

class UPrimitiveComponent;
class AActor;
UCLASS()
class INFINITERUNNER_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoin();

protected:
	virtual void BeginPlay() override;

public:
	bool bIsCollected;

	UPROPERTY(EditAnywhere)
		class USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* MeshComp;

public:
	UFUNCTION()
		void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void GetCollected();
};
