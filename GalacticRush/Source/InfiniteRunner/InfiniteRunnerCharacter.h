// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Obstacles/ObstacleBase.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "InfiniteRunnerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerCaught);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDead);

UENUM(BlueprintType)
enum class EDirection : uint8
{
	EForward UMETA(DisplayName = "Forward"),
	ELeft UMETA(DisplayName = "Left"),
	ERight UMETA(DisplayName = "Right"),
	EBackward UMETA(DisplayName = "Backward")
};

/*
* 
*/
UCLASS(config=Game)
class AInfiniteRunnerCharacter : public ACharacter
{
	GENERATED_BODY()


	//////////////////////////////////////////////////////////////////////////
    // AInfiniteRunnerCharacter


	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AInfiniteRunnerCharacter();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UPROPERTY()
		class APlayerController* PlayerController;

	UFUNCTION()
		void SetUpPlayerController();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void Tick(float DeltaSeconds) override;


	//////////////////////////////////////////////////////////////////////////
	// Player Movement


	bool bIsRunning;
	bool bIsJumping;
	bool bIsOverJumping;
	bool bIsRolling;

	FTimerHandle JumpHandle;
	FTimerHandle OverJumpHandle;
	FTimerHandle RollHandle;
	
	EDirection MovementDirection;
	bool bIsOnRightSide;

	UPROPERTY()
		FRotator DesiredTurnRotation;

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void PlayerRun();

	void PlayerJump();
	void HandleJump();

	void JumpOver();
	void HandleOverJump();

	void Roll();
	void HandleRoll();

	void TurnRight();
	void TurnLeft();

	void ConfigureTurning();

	void MoveToRightSide();
	void MoveToLeftSide();

public:
	FTimerHandle SlipHandle;
	FTimerHandle HitHardHandle;
	FTimerHandle DieHandle;

	UPROPERTY()
		float Energy;

	UPROPERTY()
		bool bIsSlipping;

	UPROPERTY()
		bool bIsHitHard;

	UPROPERTY()
		bool bIsInjured;

	UPROPERTY()
		bool bIsRunningInjured;

	UPROPERTY()
		bool bIsDead;

	UFUNCTION()
		void IncreaseEnergy(int Value);

	UFUNCTION()
		void LowerEnergy(float Value);

	UFUNCTION()
		void Die();

	UFUNCTION()
		void HandleSlip();

	UFUNCTION()
		void HandleHitHard();

	UFUNCTION()
		void HandleDie();


	//////////////////////////////////////////////////////////////////////////
	// Player Chasing


	UPROPERTY(BlueprintReadWrite)
		bool bIsCaught;

	UFUNCTION()
		void OnCaught();


	//////////////////////////////////////////////////////////////////////////
	// Player Obstacles

	FTimerHandle InjuryHandle;

	UFUNCTION()
		void HitObstacle(EObstacleType ObstacleType);

	void HitWaterLake();
	void HitFireBranch();
	void HitRock();

	void HandleInjury();


	//////////////////////////////////////////////////////////////////////////
	// Player PowerUps


	TArray<class APowerUpBase*> Powerups;

	void HealInjury();


	//////////////////////////////////////////////////////////////////////////
	// Player Score


	FTimerHandle DefeatHandle;

	UPROPERTY()
		bool bIsDefeated;

	float PlayerScore;
	int PlayerCoinCount;

	UPROPERTY(BlueprintReadWrite)
		class AMatchScoreCamera* MatchScoreCameraObjectReference;

	TSubclassOf<class AMatchScoreCamera> MatchScoreCameraClass;

	UFUNCTION()
		void CollectCoins(int Value);

	UFUNCTION(BlueprintCallable)
		class AMatchScoreCamera* FindMatchScoreCamera();

	void OnDefeated();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDefeatedEvent();

	UFUNCTION()
		void HandleDefeat();


	//////////////////////////////////////////////////////////////////////////
	// Player UMG


	UPROPERTY(EditAnywhere)
		class UMainWidget* MainWidget;
	

	//////////////////////////////////////////////////////////////////////////
	// Player Getters


	// Movement

	UFUNCTION(BlueprintCallable)
		bool GetIsRunning();

	UFUNCTION(BlueprintCallable)
		void SetIsRunning(bool IsRunning);

	UFUNCTION(BlueprintCallable)
		bool GetIsJumping();

	UFUNCTION(BlueprintCallable)
		bool GetIsOverJumping();

	UFUNCTION(BlueprintCallable)
		bool GetIsRolling();

	UFUNCTION(BlueprintCallable)
		bool GetIsCaught();

	UFUNCTION(BlueprintCallable)
		bool GetIsDead();

	UFUNCTION(BlueprintCallable)
		bool GetIsInjured();

	UFUNCTION(BlueprintCallable)
		bool GetIsRunningInjured();

	UFUNCTION(BlueprintCallable)
		bool GetIsHitHard();

	UFUNCTION(BlueprintCallable)
		bool GetIsSlipping();

	UFUNCTION(BlueprintCallable)
		bool GetIsDefeated();

	UFUNCTION(BlueprintCallable)
		void SetIsDefeated(bool IsDefeated);

	UFUNCTION(BlueprintCallable)
		class UMainWidget* GetMainWidget();

	UFUNCTION(BlueprintCallable)
		void SetMainWidget(class UMainWidget* MainWidgetObjectReference);

	// Player Stats

	UFUNCTION(BlueprintCallable)
		float GetPlayerScore();

	UFUNCTION(BlueprintCallable)
		int GetCoinCount();
		
	UFUNCTION(BlueprintCallable)
		float GetEnergy();

	// Delegates
	FOnPlayerCaught OnPlayerCaught;
	FOnPlayerDead OnPlayerDead;
};

