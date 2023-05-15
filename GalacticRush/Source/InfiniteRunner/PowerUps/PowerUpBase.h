// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpBase.generated.h"

/*
* Energy : gets you back 40.0f of energy
* Medkit : fully heal
* CoinBurst : adds 40 coins
* Magnet : captures coins
*/
UENUM(BlueprintType)
enum class EPowerUpType : uint8
{
	EEnergy UMETA(DisplayName = "Energy"),
	EMedkit UMETA(DisplayName = "Medkit"),
	ECoinBurst UMETA(DisplayName = "CoinBurst"),
	EMagic UMETA(DisplayName = "Magic")
};

UCLASS()
class INFINITERUNNER_API APowerUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APowerUpBase();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
		class USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* AttachedMesh;

	UPROPERTY(EditAnywhere)
		EPowerUpType PowerUpType;
	
	UPROPERTY(EditAnywhere)
		float PowerUpAmount;

	UPROPERTY(EditAnywhere)
		class UNiagaraComponent* Effect;
	
	UPROPERTY(EditAnywhere)
		class USoundCue* HitSound;

public:
	UFUNCTION()
		void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void ApplyPower(class AInfiniteRunnerCharacter* Character);

	UFUNCTION()
		void GetCollected();
};
