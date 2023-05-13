// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchScoreCamera.h"
#include "Camera/CameraComponent.h"

AMatchScoreCamera::AMatchScoreCamera()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	RootComponent = Camera;
}

void AMatchScoreCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMatchScoreCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

