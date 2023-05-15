// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleBase.h"
#include "WaterLake.generated.h"

/**
 * 
 */
UCLASS()
class INFINITERUNNER_API AWaterLake : public AObstacleBase
{
	GENERATED_BODY()
	
public:
	AWaterLake();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* Collision;

public:
	UFUNCTION()
		virtual void OnActorBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
