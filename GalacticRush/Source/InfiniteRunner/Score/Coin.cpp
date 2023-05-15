// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "InfiniteRunner/InfiniteRunnerCharacter.h"

ACoin::ACoin()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = Collision;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComp->SetupAttachment(Collision);

	bIsCollected = false;
}

void ACoin::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnActorBeginOverlap);
}

void ACoin::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AInfiniteRunnerCharacter* Character = Cast< AInfiniteRunnerCharacter>(OtherActor))
		{
			GetCollected();
			Character->CollectCoins(1);
		}
	}
}

void ACoin::GetCollected()
{
	bIsCollected = true;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetHiddenInGame(true);
}