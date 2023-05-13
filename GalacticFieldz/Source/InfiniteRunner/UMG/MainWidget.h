// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITERUNNER_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ScoreTextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* CoinCountTextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* EnergyTextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* InjuryIndication;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* CurrentPowerUp;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* MatchScoreTextBlock;

	UFUNCTION(BlueprintCallable)
		void SetScoreTextBlock(float Value);

	UFUNCTION(BlueprintCallable)
		void SetCoinCountTextBlock(int Value);

	UFUNCTION(BlueprintCallable)
		void SetEnergyTextBlock(float Value);

	UFUNCTION(BlueprintCallable)
		void SetMatchScoreTextBlock(const FString& MatchScore);
	
	UFUNCTION(BlueprintCallable)
		void DisplayInjuryIndication(bool IsVisible);

	UFUNCTION(BlueprintCallable)
		void DisplayCurrentPowerUp(bool IsVisible);
};
