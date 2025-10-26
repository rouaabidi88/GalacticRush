// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfiniteRunnerGameMode.generated.h"

class AInfiniteRunnerCharacter;

UCLASS()
class INFINITERUNNER_API AInfiniteRunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInfiniteRunnerGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	AInfiniteRunnerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	bool bIsGameOver;

	UFUNCTION(BlueprintCallable)
	void GameOver();

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UPROPERTY(BlueprintReadOnly, BlueprintAssignable)
	float GlobalScore;
};
