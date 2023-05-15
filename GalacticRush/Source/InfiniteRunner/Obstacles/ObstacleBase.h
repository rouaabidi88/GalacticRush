// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ObstacleBase.generated.h"

UENUM(BlueprintType)
enum class EObstacleType : uint8
{
	EFireBranch UMETA(DisplayName = "FireBranch"),
	EWaterLake UMETA(DisplayName = "WaterLake"),
	ERock UMETA(DisplayName = "Rock")
};

USTRUCT(BlueprintType)
struct FObstacleData : public FTableRowBase
{
	GENERATED_BODY()

};

UCLASS()
class INFINITERUNNER_API AObstacleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AObstacleBase();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
		class UNiagaraComponent* Effect;

	UPROPERTY(EditAnywhere)
		EObstacleType ObstacleType;
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere)
		class USoundCue* HitSound;
	
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* HitEffect;

	TSubclassOf<class AInfiniteRunnerCharacter> CharacterClass;

	UFUNCTION(BlueprintCallable)
		virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		class UAudioComponent* PlaySound();

	void DestroyObstacle();
};
