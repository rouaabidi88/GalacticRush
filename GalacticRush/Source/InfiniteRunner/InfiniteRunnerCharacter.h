// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Obstacles/ObstacleBase.h"
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

UCLASS(config = Game)
class AInfiniteRunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:
	AInfiniteRunnerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	float TurnRateGamepad;

	UPROPERTY()
	APlayerController* PlayerController;

	UFUNCTION()
	void SetUpPlayerController();

	bool bIsRunning;
	bool bIsJumping;
	bool bIsOverJumping;
	bool bIsRolling;

	FTimerHandle JumpHandle;
	FTimerHandle OverJumpHandle;
	FTimerHandle RollHandle;

	EDirection MovementDirection;
	bool bIsOnRightSide;
	FRotator DesiredTurnRotation;

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
	void IncreaseEnergy(int Value);
	void LowerEnergy(float Value);
	void Die();
	void HandleSlip();
	void HandleHitHard();
	void HandleDie();
	void OnCaught();
	void HitObstacle(EObstacleType ObstacleType);
	void HitWaterLake();
	void HitFireBranch();
	void HitRock();
	void HandleInjury();
	void HealInjury();
	void CollectCoins(int Value);
	AMatchScoreCamera* FindMatchScoreCamera();
	void OnDefeated();
	void HandleDefeat();

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
	UPROPERTY(BlueprintReadWrite)
	bool bIsCaught;

	FTimerHandle SlipHandle;
	FTimerHandle HitHardHandle;
	FTimerHandle DieHandle;
	FTimerHandle InjuryHandle;
	FTimerHandle DefeatHandle;

	TArray<class APowerUpBase*> Powerups;

	UPROPERTY()
	bool bIsDefeated;
	float PlayerScore;
	int PlayerCoinCount;
	UPROPERTY(BlueprintReadWrite)
	class AMatchScoreCamera* MatchScoreCameraObjectReference;
	TSubclassOf<class AMatchScoreCamera> MatchScoreCameraClass;
	UPROPERTY(EditAnywhere)
	class UMainWidget* MainWidget;

	FOnPlayerCaught OnPlayerCaught;
	FOnPlayerDead OnPlayerDead;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable) bool GetIsRunning() { return bIsRunning; }
	UFUNCTION(BlueprintCallable) void SetIsRunning(bool IsRunning) { bIsRunning = IsRunning; }
	UFUNCTION(BlueprintCallable) bool GetIsJumping() { return bIsJumping; }
	UFUNCTION(BlueprintCallable) bool GetIsOverJumping() { return bIsOverJumping; }
	UFUNCTION(BlueprintCallable) bool GetIsRolling() { return bIsRolling; }
	UFUNCTION(BlueprintCallable) bool GetIsCaught() { return bIsCaught; }
	UFUNCTION(BlueprintCallable) bool GetIsDead() { return bIsDead; }
	UFUNCTION(BlueprintCallable) bool GetIsInjured() { return bIsInjured; }
	UFUNCTION(BlueprintCallable) bool GetIsRunningInjured() { return bIsRunningInjured; }
	UFUNCTION(BlueprintCallable) bool GetIsHitHard() { return bIsHitHard; }
	UFUNCTION(BlueprintCallable) bool GetIsSlipping() { return bIsSlipping; }
	UFUNCTION(BlueprintCallable) bool GetIsDefeated() { return bIsDefeated; }
	UFUNCTION(BlueprintCallable) void SetIsDefeated(bool IsDefeated) { bIsDefeated = IsDefeated; }
	UFUNCTION(BlueprintCallable) UMainWidget* GetMainWidget() { return MainWidget; }
	UFUNCTION(BlueprintCallable) void SetMainWidget(UMainWidget* MainWidgetObjectReference) { MainWidget = MainWidgetObjectReference; }
	UFUNCTION(BlueprintCallable) float GetPlayerScore() { return PlayerScore; }
	UFUNCTION(BlueprintCallable) int GetCoinCount() { return PlayerCoinCount; }
	UFUNCTION(BlueprintCallable) float GetEnergy() { return Energy; }
};
