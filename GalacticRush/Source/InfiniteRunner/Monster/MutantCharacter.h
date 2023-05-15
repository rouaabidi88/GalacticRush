// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MutantCharacter.generated.h"

UCLASS()
class INFINITERUNNER_API AMutantCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMutantCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	TArray<AActor*> FoundCharacters;

	FTimerHandle BeginPlayHandle;
	FTimerHandle ChaseHandle;
	FTimerHandle MatchEndedHandle;

	bool bIsChasingPlayer;
	bool bHasCaughtPlayer;

	TSubclassOf<class AInfiniteRunnerCharacter> ChasedPlayerClass;

	UPROPERTY(BlueprintReadWrite)
		class AInfiniteRunnerCharacter* ChasedPlayer;

	FTransform TargetTransform;

public:
	void HandleBeginPlay();

	bool HasFoundPlayerToChase();

	void ChasePlayer();
	void CatchPlayer();
	void Disappear();

	UFUNCTION(BlueprintCallable)
		class AInfiniteRunnerCharacter* GetChasedPlayer();

	UFUNCTION(BlueprintCallable)
		void SetChasedPlayer(class AInfiniteRunnerCharacter* FoundPlayer);

	UFUNCTION(BlueprintCallable)
		void GetPlayerOnCaught(FTransform PlayerTransform);

	UFUNCTION(BlueprintCallable)
		void GetPlayerOnDead(FTransform PlayerTransform);

	UFUNCTION(BlueprintCallable)
		bool GetIsChasingPlayer();

	UFUNCTION(BlueprintCallable)
		bool GetHasCaughtPlayer();

	UFUNCTION()
		void OnMatchEnded();
};
