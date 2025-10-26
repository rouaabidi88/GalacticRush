// Copyright Epic Games, Inc. All Rights Reserved.

#include "Monster.h"
#include "InfiniteRunner/InfiniteRunnerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UCapsuleComponent>("Collision");
	RootComponent = Collision;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ChasedPlayerClass);
	if (FoundActor)
	{
		ChasedPlayer = Cast<AInfiniteRunnerCharacter>(FoundActor);
		if (ChasedPlayer)
		{
			ChasedPlayer->OnPlayerCaught.AddDynamic(this, &AMonster::OnPlayerCaught);
			ChasedPlayer->OnPlayerDead.AddDynamic(this, &AMonster::OnPlayerDead);
			StartChase();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Monster: Player not found to chase!"));
	}
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsChasingPlayer && ChasedPlayer && !ChasedPlayer->GetIsDead())
	{
		FVector Direction = ChasedPlayer->GetActorLocation() - GetActorLocation();
		Direction.Z = 0;
		Direction.Normalize();

		FVector NewLocation = GetActorLocation() + Direction * ChaseSpeed * DeltaTime;
		SetActorLocation(NewLocation);

		FRotator LookAtRotation = Direction.Rotation();
		SetActorRotation(LookAtRotation);

		if (FVector::Dist(GetActorLocation(), ChasedPlayer->GetActorLocation()) < 100.f)
		{
			CatchPlayer();
		}
	}
}

void AMonster::StartChase()
{
	bIsChasingPlayer = true;
	GetWorld()->GetTimerManager().SetTimer(ChaseHandle, this, &AMonster::Disappear, LifeSpanAfterChase, false);
}

void AMonster::CatchPlayer()
{
	if (ChasedPlayer && !ChasedPlayer->GetIsDead())
	{
		ChasedPlayer->OnPlayerCaught.Broadcast();
		Disappear();
	}
}

void AMonster::Disappear()
{
	bIsChasingPlayer = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void AMonster::OnPlayerCaught()
{
	UE_LOG(LogTemp, Warning, TEXT("Monster: Player has been caught!"));
	Disappear();
}

void AMonster::OnPlayerDead()
{
	UE_LOG(LogTemp, Warning, TEXT("Monster: Player is dead!"));
	Disappear();
}

bool AMonster::GetIsChasingPlayer()
{
	return bIsChasingPlayer;
}
