// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "InfiniteRunner/InfiniteRunnerCharacter.h"
#include "GameFramework/PawnMovementComponent.h"


APowerUpBase::APowerUpBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Collision = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = Collision;

	AttachedMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	Effect = CreateDefaultSubobject<UNiagaraComponent>("Effect");
	Effect->SetupAttachment(Collision);
}

void APowerUpBase::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APowerUpBase::OnActorBeginOverlap);
}

void APowerUpBase::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AInfiniteRunnerCharacter* Character = Cast<AInfiniteRunnerCharacter>(OtherActor))
		{
			ApplyPower(Character);
			GetCollected();
		}
	}
}

void APowerUpBase::ApplyPower(AInfiniteRunnerCharacter* Character)
{
	switch (PowerUpType)
	{
	case EPowerUpType::EEnergy:
	{
		Character->IncreaseEnergy(40.0f);
		break;
	}
	case EPowerUpType::EMedkit:
	{
		Character->HealInjury();
	}
	case EPowerUpType::ECoinBurst:
	{
		Character->CollectCoins(40);
	}
	case EPowerUpType::EMagic:
	{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		UE_LOG(LogTemp, Warning, TEXT("Magic"));
#endif
	}
	}
}

void APowerUpBase::GetCollected()
{
	AttachedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachedMesh->SetHiddenInGame(true);
	Effect->SetHiddenInGame(true);
}