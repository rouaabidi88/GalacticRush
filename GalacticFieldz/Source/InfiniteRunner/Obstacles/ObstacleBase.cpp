// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "InfiniteRunner/InfiniteRunnerCharacter.h"

#include "Kismet/GameplayStatics.h"

#include "FireBranch.h"
#include "WaterLake.h"
#include "Rock.h"


AObstacleBase::AObstacleBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	Effect = CreateDefaultSubobject<UNiagaraComponent>("Effect");
}

void AObstacleBase::BeginPlay()
{
	Super::BeginPlay();
}

void AObstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AObstacleBase::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

UAudioComponent* AObstacleBase::PlaySound()
{
	UAudioComponent* AudioComp = NULL;
	if (HitSound)
	{
		//AudioComp = UGameplayStatics::SpawnSoundAttached(HitSound, MeshComp, FName(""));
		//AudioComp = UGameplayStatics::SpawnSoundAtLocation();
	}
	return AudioComp;
}

void AObstacleBase::DestroyObstacle()
{
	Destroy();
}


