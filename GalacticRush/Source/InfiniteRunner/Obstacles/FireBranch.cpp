// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBranch.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"

#include "InfiniteRunner/InfiniteRunnerCharacter.h"

AFireBranch::AFireBranch()
{
	Collision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	RootComponent = Collision;

	Collision->SetNotifyRigidBodyCollision(true);

	MeshComp->SetupAttachment(Collision);
	Effect->SetupAttachment(Collision);

	ObstacleType = EObstacleType::EFireBranch;
}

void AFireBranch::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AFireBranch::OnActorBeginOverlap);
}

void AFireBranch::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AInfiniteRunnerCharacter* Player = Cast<AInfiniteRunnerCharacter>(OtherActor))
		{
			Player->HitObstacle(ObstacleType);
		}
	}
}


