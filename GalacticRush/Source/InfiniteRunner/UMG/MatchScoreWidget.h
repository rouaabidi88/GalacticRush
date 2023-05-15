// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITERUNNER_API UMatchScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
};
