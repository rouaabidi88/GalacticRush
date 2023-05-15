// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"

#include "Kismet/KismetTextLibrary.h"
#include "MatchScoreWidget.h"

void UMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetScoreTextBlock(0.0f);
	SetCoinCountTextBlock(0);
	SetEnergyTextBlock(100.0f);

	MatchScoreTextBlock->SetVisibility(ESlateVisibility::Hidden);

	DisplayInjuryIndication(false);
	DisplayCurrentPowerUp(false);
}

void UMainWidget::SetScoreTextBlock(float Value)
{
	ScoreTextBlock->SetText(FText::FromString(FString::SanitizeFloat(Value)));
}

void UMainWidget::SetCoinCountTextBlock(int Value)
{
	CoinCountTextBlock->SetText(FText::FromString(FString::FromInt(Value)));
}

void UMainWidget::SetEnergyTextBlock(float Value)
{
	EnergyTextBlock->SetText(FText::FromString(FString::SanitizeFloat(Value)));
}

void UMainWidget::SetMatchScoreTextBlock(const FString& MatchScore)
{
	MatchScoreTextBlock->SetVisibility(ESlateVisibility::Visible);
	if (MatchScore.Equals("Lost"))
	{
		MatchScoreTextBlock->SetText(FText::FromString(MatchScore));
	}
}

void UMainWidget::DisplayInjuryIndication(bool IsVisible)
{
	if (IsVisible)
	{
		InjuryIndication->SetVisibility(ESlateVisibility::Visible);
	}

	else
	{
		InjuryIndication->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainWidget::DisplayCurrentPowerUp(bool IsVisible)
{
	if (IsVisible)
	{
		CurrentPowerUp->SetVisibility(ESlateVisibility::Visible);
	}

	else
	{
		CurrentPowerUp->SetVisibility(ESlateVisibility::Hidden);
	}
}