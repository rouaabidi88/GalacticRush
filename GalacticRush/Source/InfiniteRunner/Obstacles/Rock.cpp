// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

#include "InfiniteRunner/InfiniteRunnerCharacter.h"

ARock::ARock()
{
	Collision = CreateDefaultSubobject<USphereComponent>("BoxComponent");
	RootComponent = Collision;

	MeshComp->SetupAttachment(Collision);
	Effect->SetupAttachment(Collision);

	ObstacleType = EObstacleType::ERock;
}

void ARock::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ARock::OnActorBeginOverlap);
}

void ARock::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AInfiniteRunnerCharacter* Player = Cast<AInfiniteRunnerCharacter>(OtherActor))
		{
			Player->HitObstacle(ObstacleType);
		}
	}
}
