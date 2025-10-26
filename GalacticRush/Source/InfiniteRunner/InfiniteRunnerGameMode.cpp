// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfiniteRunnerGameMode.h"
#include "InfiniteRunnerCharacter.h"
#include "Kismet/GameplayStatics.h"

AInfiniteRunnerGameMode::AInfiniteRunnerGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_InfiniteRunnerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	bIsGameOver = false;
	GlobalScore = 0.f;
}

void AInfiniteRunnerGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AInfiniteRunnerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (PlayerCharacter)
	{
		PlayerCharacter->OnPlayerDead.AddDynamic(this, &AInfiniteRunnerGameMode::GameOver);
	}
}

void AInfiniteRunnerGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsGameOver && PlayerCharacter)
	{
		GlobalScore = PlayerCharacter->GetPlayerScore();
	}
}

void AInfiniteRunnerGameMode::GameOver()
{
	if (bIsGameOver) return;

	bIsGameOver = true;
	if (PlayerCharacter)
	{
		PlayerCharacter->bIsRunning = false;
		PlayerCharacter->bIsRunningInjured = false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Game Over! Final Score: %f"), GlobalScore);
}

void AInfiniteRunnerGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
