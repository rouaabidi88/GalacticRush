// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monster.generated.h"

UCLASS()
class INFINITERUNNER_API AMonster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:

	FTimerHandle ChaseHandle;

	bool bIsChasingPlayer;


	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* Collision;

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* MeshComp;

	TSubclassOf<class AInfiniteRunnerCharacter> ChasedPlayerClass;

	UPROPERTY(EditAnywhere)
		class AInfiniteRunnerCharacter* ChasedPlayer;
public:

	bool GetChasedPlayer();

	void ChasePlayer();
	void CatchPlayer();
	void Disappear();
	
	UFUNCTION(BlueprintCallable)
		void GetPlayerOnCaught();

	UFUNCTION(BlueprintCallable)
		void GetPlayerOnDead();

	UFUNCTION(BlueprintCallable)
		bool GetIsChasingPlayer();
};
