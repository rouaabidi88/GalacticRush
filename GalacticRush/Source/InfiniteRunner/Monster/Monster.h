// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monster.generated.h"

class AInfiniteRunnerCharacter;

UCLASS()
class INFINITERUNNER_API AMonster : public AActor
{
	GENERATED_BODY()

public:
	AMonster();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	TSubclassOf<AInfiniteRunnerCharacter> ChasedPlayerClass;

	UPROPERTY(BlueprintReadOnly)
	AInfiniteRunnerCharacter* ChasedPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float ChaseSpeed = 800.f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsChasingPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float LifeSpanAfterChase = 4.0f;

	FTimerHandle ChaseHandle;

	UFUNCTION(BlueprintCallable)
	void StartChase();

	UFUNCTION(BlueprintCallable)
	void Disappear();

	void CatchPlayer();

	UFUNCTION()
	void OnPlayerCaught();

	UFUNCTION()
	void OnPlayerDead();
};
