// Fill out your copyright notice in the Description page of Project Settings.


#include "MutantCharacter.h"
#include "InfiniteRunner/InfiniteRunnerCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"

AMutantCharacter::AMutantCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsChasingPlayer = false;
	bHasCaughtPlayer = false;
}

void AMutantCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(BeginPlayHandle, this, &AMutantCharacter::HandleBeginPlay, 8.0f, false);
}

void AMutantCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsChasingPlayer)
	{
		TargetTransform = ChasedPlayer->GetActorTransform();
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetTransform.GetLocation(), 1.0f, 100.0f);

		SetActorLocation(NewLocation);
	}
}

void AMutantCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMutantCharacter::HandleBeginPlay()
{
	if (ChasedPlayer)
	{
		ChasePlayer();
	}
}

bool AMutantCharacter::HasFoundPlayerToChase()
{
	/*
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ChasedPlayerClass, FoundCharacters);
	UE_LOG(LogTemp, Warning, TEXT("Found character : %d"), FoundCharacters.Num());

	if (FoundCharacters.Num() > 0)
	{
		if (AInfiniteRunnerCharacter* FoundChasedPlayer = Cast<AInfiniteRunnerCharacter>(FoundCharacters[0]))
		{
			ChasedPlayer = FoundChasedPlayer;
			ChasedPlayer->OnPlayerCaught.AddDynamic(this, &AMutantCharacter::GetPlayerOnCaught);
			ChasedPlayer->OnPlayerDead.AddDynamic(this, &AMutantCharacter::GetPlayerOnDead);
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("player to chase unfound!"));
			return false;
		}
	}
	*/
	return false;
}

void AMutantCharacter::ChasePlayer()
{
	bIsChasingPlayer = true;
	GetWorld()->GetTimerManager().SetTimer(ChaseHandle, this, &AMutantCharacter::Disappear, 4.0f, false);
}

void AMutantCharacter::CatchPlayer()
{
	TargetTransform = ChasedPlayer->GetActorTransform();
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetTransform.GetLocation(), 1.0f, 100.0f);

	SetActorLocation(NewLocation);

	bHasCaughtPlayer = true;

	GetWorld()->GetTimerManager().SetTimer(MatchEndedHandle, this, &AMutantCharacter::OnMatchEnded, 6.0f, false);
}

void AMutantCharacter::Disappear()
{
	bIsChasingPlayer = false;
	SetActorHiddenInGame(true);
}

AInfiniteRunnerCharacter* AMutantCharacter::GetChasedPlayer()
{
	return ChasedPlayer;
}

void AMutantCharacter::SetChasedPlayer(AInfiniteRunnerCharacter* Player)
{
	ChasedPlayer = Player;
}

void AMutantCharacter::GetPlayerOnCaught(FTransform PlayerTransform)
{
	CatchPlayer();
}

void AMutantCharacter::GetPlayerOnDead(FTransform PlayerTransform)
{
	CatchPlayer();
}

bool AMutantCharacter::GetIsChasingPlayer()
{
	return bIsChasingPlayer;
}

bool AMutantCharacter::GetHasCaughtPlayer()
{
	return bHasCaughtPlayer;
}

void AMutantCharacter::OnMatchEnded()
{
	bIsChasingPlayer = false;
	bHasCaughtPlayer = false;
}
