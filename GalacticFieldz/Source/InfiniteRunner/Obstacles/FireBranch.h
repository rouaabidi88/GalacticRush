// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleBase.h"
#include "FireBranch.generated.h"

class UPrimitiveComponent;
/**
 * 
 */
UCLASS()
class INFINITERUNNER_API AFireBranch : public AObstacleBase
{
	GENERATED_BODY()

public:
	AFireBranch();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* Collision;

public:
	UFUNCTION(BlueprintCallable)
		virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};

