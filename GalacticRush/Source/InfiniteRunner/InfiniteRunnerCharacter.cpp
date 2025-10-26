// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfiniteRunnerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "InfiniteRunner/Match/MatchScoreCamera.h"
#include "UMG/MainWidget.h"

AInfiniteRunnerCharacter::AInfiniteRunnerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	TurnRateGamepad = 50.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 500, 0);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	MovementDirection = EDirection::EForward;
	bIsRunning = true;
	bIsJumping = bIsOverJumping = bIsRolling = false;
	bIsOnRightSide = true;
	DesiredTurnRotation = FRotator::ZeroRotator;
	bIsSlipping = bIsHitHard = bIsInjured = bIsRunningInjured = bIsDead = false;
	Energy = 100.f;
	PlayerScore = 0.f;
	PlayerCoinCount = 0;
}

void AInfiniteRunnerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetUpPlayerController();
}

void AInfiniteRunnerCharacter::SetUpPlayerController()
{
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void AInfiniteRunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AInfiniteRunnerCharacter::PlayerJump);
	PlayerInputComponent->BindAction("OverJump", IE_Pressed, this, &AInfiniteRunnerCharacter::JumpOver);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AInfiniteRunnerCharacter::Roll);
	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &AInfiniteRunnerCharacter::TurnRight);
	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &AInfiniteRunnerCharacter::TurnLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AInfiniteRunnerCharacter::MoveToRightSide);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AInfiniteRunnerCharacter::MoveToLeftSide);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AInfiniteRunnerCharacter::MoveRight);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AInfiniteRunnerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AInfiniteRunnerCharacter::TouchStopped);
}

void AInfiniteRunnerCharacter::Tick(float DeltaSeconds)
{
	if (bIsRunning || bIsRunningInjured)
	{
		ConfigureTurning();
		PlayerScore += 1.f;
		if (MainWidget) MainWidget->SetScoreTextBlock(PlayerScore);
		MoveForward(1.f);
	}
}

void AInfiniteRunnerCharacter::PlayerRun() { bIsRunning = true; }

void AInfiniteRunnerCharacter::PlayerJump()
{
	Super::Jump();
	bIsJumping = true;
	GetWorld()->GetTimerManager().SetTimer(JumpHandle, this, &AInfiniteRunnerCharacter::HandleJump, 0.43f, false);
}

void AInfiniteRunnerCharacter::HandleJump() { bIsJumping = false; }

void AInfiniteRunnerCharacter::JumpOver()
{
	bIsOverJumping = true;
	GetWorld()->GetTimerManager().SetTimer(OverJumpHandle, this, &AInfiniteRunnerCharacter::HandleOverJump, 1.8f, false);
}

void AInfiniteRunnerCharacter::HandleOverJump() { bIsOverJumping = false; }

void AInfiniteRunnerCharacter::Roll()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(48);
	bIsRolling = true;
	GetWorld()->GetTimerManager().SetTimer(RollHandle, this, &AInfiniteRunnerCharacter::HandleRoll, 0.9f, false);
}

void AInfiniteRunnerCharacter::HandleRoll()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(96);
	bIsRolling = false;
}

void AInfiniteRunnerCharacter::TurnRight()
{
	DesiredTurnRotation += FRotator(0, 90, 0);
	switch (MovementDirection)
	{
	case EDirection::EForward: MovementDirection = EDirection::ERight; break;
	case EDirection::ELeft: MovementDirection = EDirection::EForward; break;
	case EDirection::ERight: MovementDirection = EDirection::EBackward; break;
	case EDirection::EBackward: MovementDirection = EDirection::ELeft; break;
	}
}

void AInfiniteRunnerCharacter::TurnLeft()
{
	DesiredTurnRotation += FRotator(0, -90, 0);
	switch (MovementDirection)
	{
	case EDirection::EForward: MovementDirection = EDirection::ELeft; break;
	case EDirection::ELeft: MovementDirection = EDirection::EBackward; break;
	case EDirection::ERight: MovementDirection = EDirection::EForward; break;
	case EDirection::EBackward: MovementDirection = EDirection::ERight; break;
	}
}

void AInfiniteRunnerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location) { Jump(); }
void AInfiniteRunnerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location) { StopJumping(); }

void AInfiniteRunnerCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(FRotator(0, Rotation.Yaw, 0)).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AInfiniteRunnerCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(FRotator(0, Rotation.Yaw, 0)).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AInfiniteRunnerCharacter::ConfigureTurning()
{
	if (!PlayerController) return;
	FRotator NewRotation = UKismetMathLibrary::RInterpTo(PlayerController->GetControlRotation(), DesiredTurnRotation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 5.f);
	PlayerController->SetControlRotation(NewRotation);
}

void AInfiniteRunnerCharacter::MoveToRightSide()
{
	if (!bIsOnRightSide)
	{
		FVector Offset = FVector::ZeroVector;
		switch (MovementDirection)
		{
		case EDirection::EForward: Offset.Y = 100; break;
		case EDirection::EBackward: Offset.Y = -100; break;
		case EDirection::ERight: Offset.X = -100; break;
		case EDirection::ELeft: Offset.X = 100; break;
		}
		SetActorLocation(GetActorLocation() + Offset);
		bIsOnRightSide = true;
	}
}

void AInfiniteRunnerCharacter::MoveToLeftSide()
{
	if (bIsOnRightSide)
	{
		FVector Offset = FVector::ZeroVector;
		switch (MovementDirection)
		{
		case EDirection::EForward: Offset.Y = -100; break;
		case EDirection::EBackward: Offset.Y = 100; break;
		case EDirection::ERight: Offset.X = 100; break;
		case EDirection::ELeft: Offset.X = -100; break;
		}
		SetActorLocation(GetActorLocation() + Offset);
		bIsOnRightSide = false;
	}
}

void AInfiniteRunnerCharacter::IncreaseEnergy(int Value)
{
	Energy = FMath::Clamp(Energy + Value, 0.f, 100.f);
	if (Energy == 100.f && bIsRunningInjured) HealInjury();
	if (MainWidget) MainWidget->SetEnergyTextBlock(Energy);
}

void AInfiniteRunnerCharacter::LowerEnergy(float Value)
{
	Energy -= Value;
	if (MainWidget) MainWidget->SetEnergyTextBlock(Energy);
}

void AInfiniteRunnerCharacter::Die()
{
	GetCharacterMovement()->DisableMovement();
}

void AInfiniteRunnerCharacter::HandleSlip() { bIsSlipping = false; OnDefeatedEvent(); }
void AInfiniteRunnerCharacter::HandleHitHard() { bIsHitHard = false; OnDefeatedEvent(); }
void AInfiniteRunnerCharacter::HandleDie() { bIsDead = false; OnDefeatedEvent(); }

void AInfiniteRunnerCharacter::OnCaught() { bIsRunning = false; }

void AInfiniteRunnerCharacter::HitObstacle(EObstacleType ObstacleType)
{
	bIsRunning = false;
	switch (ObstacleType)
	{
	case EObstacleType::EWaterLake: HitWaterLake(); break;
	case EObstacleType::EFireBranch: HitFireBranch(); break;
	default: HitRock(); break;
	}
}

void AInfiniteRunnerCharacter::HitWaterLake()
{
	bIsSlipping = true;
	GetWorld()->GetTimerManager().SetTimer(SlipHandle, this, &AInfiniteRunnerCharacter::HandleSlip, 1.6f, false);
}

void AInfiniteRunnerCharacter::HitFireBranch()
{
	bIsHitHard = true;
	GetWorld()->GetTimerManager().SetTimer(HitHardHandle, this, &AInfiniteRunnerCharacter::HandleHitHard, 1.6f, false);
}

void AInfiniteRunnerCharacter::HitRock()
{
	LowerEnergy(50.f);
	if (bIsRunningInjured)
	{
		bIsRunningInjured = false;
		bIsDead = true;
		GetWorld()->GetTimerManager().SetTimer(DieHandle, this, &AInfiniteRunnerCharacter::HandleDie, 1.6f, false);
	}
	else
	{
		bIsInjured = true;
		if (MainWidget) MainWidget->DisplayInjuryIndication(true);
		GetWorld()->GetTimerManager().SetTimer(InjuryHandle, this, &AInfiniteRunnerCharacter::HandleInjury, 1.2f, false);
	}
}

void AInfiniteRunnerCharacter::HandleInjury()
{
	bIsInjured = false;
	if (bIsOnRightSide) MoveToLeftSide();
	else MoveToRightSide();
	bIsRunningInjured = true;
}

void AInfiniteRunnerCharacter::HealInjury()
{
	bIsRunningInjured = false;
	Energy = 100.f;
	if (MainWidget) MainWidget->DisplayInjuryIndication(false);
	PlayerRun();
}

void AInfiniteRunnerCharacter::CollectCoins(int Value)
{
	PlayerCoinCount += Value;
	if (MainWidget) MainWidget->SetCoinCountTextBlock(PlayerCoinCount);
}

AMatchScoreCamera* AInfiniteRunnerCharacter::FindMatchScoreCamera()
{
	if (AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), MatchScoreCameraClass))
		return Cast<AMatchScoreCamera>(Found);
	return nullptr;
}

void AInfiniteRunnerCharacter::OnDefeated()
{
	SetActorTransform(FTransform(FRotator::ZeroRotator, FVector(1960, 2920, 96), FVector(1.f)));
	if (AMatchScoreCamera* Camera = FindMatchScoreCamera())
	{
		MatchScoreCameraObjectReference = Camera;
		PlayerController->SetViewTargetWithBlend(MatchScoreCameraObjectReference, 5.f);
	}
	bIsDefeated = true;
	GetWorld()->GetTimerManager().SetTimer(DefeatHandle, this, &AInfiniteRunnerCharacter::HandleDefeat, 1.6f, false);
}

void AInfiniteRunnerCharacter::HandleDefeat() { bIsDefeated = false; }

