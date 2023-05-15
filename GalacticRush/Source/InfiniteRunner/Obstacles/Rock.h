// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleBase.h"
#include "Rock.generated.h"

/**
 * 
 */
UCLASS()
class INFINITERUNNER_API ARock : public AObstacleBase
{
	GENERATED_BODY()
	
public:
	ARock();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
		class USphereComponent* Collision;

public:

	UFUNCTION()
		virtual void OnActorBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
