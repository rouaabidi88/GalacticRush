// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "InfiniteRunner/InfiniteRunnerCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	RootComponent = Collision;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	MeshComp->SetupAttachment(RootComponent);

}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetChasedPlayer())
	{
		bIsChasingPlayer = true;
		GetWorld()->GetTimerManager().SetTimer(ChaseHandle, this, &AMonster::Disappear, 4.0f, false);
	}
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsChasingPlayer)
	{
		
	}
}

bool AMonster::GetChasedPlayer()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ChasedPlayerClass);

	if (AInfiniteRunnerCharacter* FoundChasedPlayer = Cast<AInfiniteRunnerCharacter>(FoundActor))
	{
		ChasedPlayer = FoundChasedPlayer;
		ChasedPlayer->OnPlayerCaught.AddDynamic(this, &AMonster::GetPlayerOnCaught);
		ChasedPlayer->OnPlayerDead.AddDynamic(this, &AMonster::GetPlayerOnDead);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("player to chase unfound!"));
		return false;
	}
}

void AMonster::ChasePlayer()
{
	//MoveComponent
}

void AMonster::CatchPlayer()
{

}

void AMonster::Disappear()
{
	bIsChasingPlayer = false;
	SetActorHiddenInGame(true);
}

void AMonster::GetPlayerOnCaught()
{

}

void AMonster::GetPlayerOnDead()
{

}

bool AMonster::GetIsChasingPlayer()
{
	return bIsChasingPlayer;
}
