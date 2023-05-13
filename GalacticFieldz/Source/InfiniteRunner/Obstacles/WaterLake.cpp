// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterLake.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"

#include "InfiniteRunner/InfiniteRunnerCharacter.h"

AWaterLake::AWaterLake()
{
	Collision = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	RootComponent = Collision;

	MeshComp->SetupAttachment(Collision);
	Effect->SetupAttachment(Collision);

	ObstacleType = EObstacleType::EWaterLake;
}

void AWaterLake::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AWaterLake::OnActorBeginOverlap);
}

void AWaterLake::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AInfiniteRunnerCharacter* Player = Cast<AInfiniteRunnerCharacter>(OtherActor))
		{
			Collision->SetCollisionProfileName("NoCollision");
			Player->HitObstacle(ObstacleType);
		}
	}
}


