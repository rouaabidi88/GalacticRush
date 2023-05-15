// Copyright Epic Games, Inc. All Rights Reserved.

#include "InfiniteRunnerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Engine/EngineTypes.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"

#include "Obstacles/ObstacleBase.h"

#include "InfiniteRunner/Match/MatchScoreCamera.h"

#include "UMG/MainWidget.h"
#include "UMG/MatchScoreWidget.h"

#include "Runtime/Core/Public/Delegates/Delegate.h"

AInfiniteRunnerCharacter::AInfiniteRunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	MovementDirection = EDirection::EForward;

	bIsRunning = true;
	bIsJumping = false;
	bIsOverJumping = false;
	bIsRolling = false;
	bIsOnRightSide = true;
	DesiredTurnRotation = FRotator(0, 0, 0);

	bIsSlipping = false;
	bIsHitHard = false;
	bIsInjured = false;
	bIsRunningInjured = false;
	bIsDead = false;

	Energy = 100.0f;

	PlayerScore = 0.0f;
	PlayerCoinCount = 0;
}

void AInfiniteRunnerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetUpPlayerController();
}

void AInfiniteRunnerCharacter::SetUpPlayerController()
{
	APlayerController* PlayerControllerReference = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController = PlayerControllerReference;
}

void AInfiniteRunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AInfiniteRunnerCharacter::PlayerJump);
	PlayerInputComponent->BindAction("OverJump", IE_Pressed, this, &AInfiniteRunnerCharacter::JumpOver);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AInfiniteRunnerCharacter::Roll);
	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &AInfiniteRunnerCharacter::TurnRight);
	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &AInfiniteRunnerCharacter::TurnLeft);

	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AInfiniteRunnerCharacter::MoveToRightSide);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AInfiniteRunnerCharacter::MoveToLeftSide);

	//PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AInfiniteRunnerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AInfiniteRunnerCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AInfiniteRunnerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AInfiniteRunnerCharacter::TouchStopped);
}

void AInfiniteRunnerCharacter::Tick(float DeltaSeconds)
{
	if (bIsRunning || bIsRunningInjured)
	{
		if (GetControlRotation() != DesiredTurnRotation)
		{
			ConfigureTurning();
		}
		PlayerScore += 1.0f;
		MainWidget->SetScoreTextBlock(PlayerScore);
		MoveForward(1.0f);
	}
}

void AInfiniteRunnerCharacter::PlayerRun()
{
	bIsRunning = true;
}

void AInfiniteRunnerCharacter::PlayerJump()
{
	Super::Jump();
	bIsJumping = true;
	GetWorld()->GetTimerManager().SetTimer(JumpHandle, this, &AInfiniteRunnerCharacter::HandleJump, 0.43f, false);
}

void AInfiniteRunnerCharacter::HandleJump()
{
	bIsJumping = false;
}

void AInfiniteRunnerCharacter::JumpOver()
{
	bIsOverJumping = true;
	GetWorld()->GetTimerManager().SetTimer(OverJumpHandle, this, &AInfiniteRunnerCharacter::HandleOverJump, 1.8f, false);
}

void AInfiniteRunnerCharacter::HandleOverJump()
{
	bIsOverJumping = false;
}

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
	DesiredTurnRotation = UKismetMathLibrary::ComposeRotators(DesiredTurnRotation, FRotator(0, 90, 0));

	switch (MovementDirection)
	{
	case EDirection::EForward:
		MovementDirection = EDirection::ERight;
		break;
	case EDirection::ELeft:
		MovementDirection = EDirection::EForward;
		break;
	case EDirection::ERight:
		MovementDirection = EDirection::EBackward;
		break;
	case EDirection::EBackward:
		MovementDirection = EDirection::ELeft;
		break;
	default:
		break;
	}
}

void AInfiniteRunnerCharacter::TurnLeft()
{
	DesiredTurnRotation = UKismetMathLibrary::ComposeRotators(DesiredTurnRotation, FRotator(0, -90, 0));

	switch (MovementDirection)
	{
	case EDirection::EForward:
		MovementDirection = EDirection::ELeft;
		break;
	case EDirection::ELeft:
		MovementDirection = EDirection::EBackward;
		break;
	case EDirection::ERight:
		MovementDirection = EDirection::EForward;
		break;
	case EDirection::EBackward:
		MovementDirection = EDirection::ERight;
		break;
	default:
		break;
	}
}

void AInfiniteRunnerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AInfiniteRunnerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AInfiniteRunnerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AInfiniteRunnerCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AInfiniteRunnerCharacter::ConfigureTurning()
{
	FRotator UpdatedRotator = UKismetMathLibrary::RInterpTo(GetControlRotation(), DesiredTurnRotation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 5.0f);
	PlayerController->SetControlRotation(UpdatedRotator);
}

void AInfiniteRunnerCharacter::MoveToRightSide()
{
	if (!bIsOnRightSide)
	{
		// can move to right side
		switch (MovementDirection)
		{
		case EDirection::EForward:
		{
			FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 100, GetActorLocation().Z);
			SetActorLocation(NewLocation);
			break;
		}
		case EDirection::EBackward:
		{
			FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y - 100, GetActorLocation().Z);
			SetActorLocation(NewLocation);
			break;
		}
		case EDirection::ERight:
		{
			FVector NewLocation = FVector(GetActorLocation().X - 100, GetActorLocation().Y, GetActorLocation().Z);
			SetActorLocation(NewLocation);
			break;
		}
		case EDirection::ELeft:
		{
			FVector NewLocation = FVector(GetActorLocation().X + 100, GetActorLocation().Y, GetActorLocation().Z);
			SetActorLocation(NewLocation);
			break;
		}
		bIsOnRightSide = true;
		}
	}
	else
	{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		UE_LOG(LogTemp, Warning, TEXT("Already on right side!"));
#endif
	}
}

void AInfiniteRunnerCharacter::MoveToLeftSide()
{
	if (bIsOnRightSide)
	{
		switch (MovementDirection)
		{
		case EDirection::EForward:
		{
			FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y - 100, GetActorLocation().Z);
			SetActorLocation(NewLocation);
			break;
		}
		case EDirection::EBackward:
		{
			FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 100, GetActorLocation().Z);
			SetActorLocation(NewLocation);
			break;
		}
		case EDirection::ERight:
		{
			FVector NewLocation = FVector(GetActorLocation().X + 100, GetActorLocation().Y, GetActorLocation().Z);
			SetActorLocation(NewLocation);
			break;
		}
		case EDirection::ELeft:
		{
			FVector NewLocation = FVector(GetActorLocation().X - 100, GetActorLocation().Y, GetActorLocation().Z);
			SetActorLocation(NewLocation);
			break;
		}
		}
		bIsOnRightSide = false;
	}
	
	else
	{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		UE_LOG(LogTemp, Warning, TEXT("Already on left side!"));
#endif
	}
}

void AInfiniteRunnerCharacter::IncreaseEnergy(int Value)
{
	if (Energy < 100.0f)
	{
		if (Energy + Value > 100.0f)
		{
			Energy = 100.0f;
		}
		else
		{
			Energy += Value;
		}
		
		if (Energy == 100.0f)
		{
			if (bIsRunningInjured)
			{
				HealInjury();
			}
		}
	}
	MainWidget->SetEnergyTextBlock(Energy);
}

void AInfiniteRunnerCharacter::LowerEnergy(float Value)
{
	Energy -= Value;
	MainWidget->SetEnergyTextBlock(Energy);
	if (Energy < 10.0f)
	{
		// affect player velocity
	}
}

void AInfiniteRunnerCharacter::Die()
{
	// disable movement
	GetCharacterMovement()->DisableMovement();
}

void AInfiniteRunnerCharacter::HandleSlip()
{
	bIsSlipping = false;
	OnDefeatedEvent();
}

void AInfiniteRunnerCharacter::HandleHitHard()
{
	bIsHitHard = false;
	OnDefeatedEvent();
}

void AInfiniteRunnerCharacter::HandleDie()
{
	bIsDead = false;
	OnDefeatedEvent();
}

void AInfiniteRunnerCharacter::OnCaught()
{
	bIsRunning = false;
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UE_LOG(LogTemp, Warning, TEXT("Caught!"));
#endif
}

void AInfiniteRunnerCharacter::HitObstacle(EObstacleType ObstacleType)
{
	bIsRunning = false;
	if (ObstacleType == EObstacleType::EWaterLake)
	{
		// slip and lose
		HitWaterLake();

		// TODO : get mutant
	}

	else if (ObstacleType == EObstacleType::EFireBranch)
	{
		// die and lose
		HitFireBranch();
	}

	// Hit Rock
	else
	{
		// get injured and if already injured die
		HitRock();
	}
}

void AInfiniteRunnerCharacter::HitWaterLake()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UE_LOG(LogTemp, Warning, TEXT("Hit Water Lake"));
#endif
	bIsSlipping = true;
	GetWorld()->GetTimerManager().SetTimer(SlipHandle, this, &AInfiniteRunnerCharacter::HandleSlip, 1.6f, false);
}

void AInfiniteRunnerCharacter::HitFireBranch()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UE_LOG(LogTemp, Warning, TEXT("Hit Fire Branch"));
#endif
	bIsHitHard = true;
	GetWorld()->GetTimerManager().SetTimer(HitHardHandle, this, &AInfiniteRunnerCharacter::HandleHitHard, 1.6f, false);
}

void AInfiniteRunnerCharacter::HitRock()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UE_LOG(LogTemp, Warning, TEXT("Hit Rock"));
#endif
	LowerEnergy(50.0f);

	if (bIsRunningInjured)
	{
		// double injury
		bIsRunningInjured = false;
		bIsDead = true;
		GetWorld()->GetTimerManager().SetTimer(DieHandle, this, &AInfiniteRunnerCharacter::HandleDie, 1.6f, false);
	}

	else
	{
		// get injured and slow down speed
		bIsInjured = true;
		MainWidget->DisplayInjuryIndication(true);
		GetWorld()->GetTimerManager().SetTimer(InjuryHandle, this, &AInfiniteRunnerCharacter::HandleInjury, 1.2f, false);
	}
}

void AInfiniteRunnerCharacter::HandleInjury()
{
	bIsInjured = false;

	if (bIsOnRightSide)
	{
		MoveToLeftSide();
	}
	else
	{
		MoveToRightSide();
	}

	bIsRunningInjured = true;
}

void AInfiniteRunnerCharacter::HealInjury()
{
	if (bIsRunningInjured)
	{
		bIsRunningInjured = false;
		Energy = 100.0f;
		MainWidget->DisplayInjuryIndication(false);
		PlayerRun();
	}
}

void AInfiniteRunnerCharacter::CollectCoins(int Value)
{
	PlayerCoinCount += Value;
	MainWidget->SetCoinCountTextBlock(PlayerCoinCount);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	UE_LOG(LogTemp, Warning, TEXT("Coin Count : %d"), PlayerCoinCount);
#endif
}

AMatchScoreCamera* AInfiniteRunnerCharacter::FindMatchScoreCamera()
{
	if (AActor* FoundCamera = UGameplayStatics::GetActorOfClass(GetWorld(), MatchScoreCameraClass))
	{
		if (AMatchScoreCamera* MatchScoreCamera = Cast<AMatchScoreCamera>(FoundCamera))
		{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			UE_LOG(LogTemp, Warning, TEXT("Found match score camera actor"));
#endif
			return MatchScoreCamera;
		}
	}

	else
	{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		UE_LOG(LogTemp, Warning, TEXT("Camera not found!"));
#endif
	}
	return nullptr;
}

void AInfiniteRunnerCharacter::OnDefeated()
{
	FTransform NewTransform;
	NewTransform.SetLocation(FVector(1960, 2920, 96));
	NewTransform.SetRotation(FQuat::MakeFromRotator(FRotator(0, 0, 0)));
	NewTransform.SetScale3D(FVector(1, 1, 1));

	SetActorTransform(NewTransform);

	if (FindMatchScoreCamera())
	{
		MatchScoreCameraObjectReference = FindMatchScoreCamera();
		PlayerController->SetViewTargetWithBlend(MatchScoreCameraObjectReference, 5.0f);
	}

	bIsDefeated = true;
	GetWorld()->GetTimerManager().SetTimer(DefeatHandle, this, &AInfiniteRunnerCharacter::HandleDefeat, 1.6f, false);
}

void AInfiniteRunnerCharacter::HandleDefeat()
{
	bIsDefeated = false;
}

bool AInfiniteRunnerCharacter::GetIsRunning()
{
	return bIsRunning;
}

void AInfiniteRunnerCharacter::SetIsRunning(bool IsRunning)
{
	bIsRunning = IsRunning;
}

bool AInfiniteRunnerCharacter::GetIsJumping()
{
	return bIsJumping;
}

bool AInfiniteRunnerCharacter::GetIsOverJumping()
{
	return bIsOverJumping;
}

bool AInfiniteRunnerCharacter::GetIsRolling()
{
	return bIsRolling;
}

bool AInfiniteRunnerCharacter::GetIsCaught()
{
	return bIsCaught;
}

bool AInfiniteRunnerCharacter::GetIsDead()
{
	return bIsDead;
}

bool AInfiniteRunnerCharacter::GetIsInjured()
{
	return bIsInjured;
}

bool AInfiniteRunnerCharacter::GetIsRunningInjured()
{
	return bIsRunningInjured;
}

bool AInfiniteRunnerCharacter::GetIsHitHard()
{
	return bIsHitHard;
}

bool AInfiniteRunnerCharacter::GetIsSlipping()
{
	return bIsSlipping;
}

bool AInfiniteRunnerCharacter::GetIsDefeated()
{
	return bIsDefeated;
}

void AInfiniteRunnerCharacter::SetIsDefeated(bool IsDefeated)
{
	bIsDefeated = IsDefeated;
}

UMainWidget* AInfiniteRunnerCharacter::GetMainWidget()
{
	if (MainWidget)
		return MainWidget;
	return nullptr;
}

void AInfiniteRunnerCharacter::SetMainWidget(UMainWidget* MainWidgetObjectReference)
{
	if (MainWidgetObjectReference)
		MainWidget = MainWidgetObjectReference;
}

float AInfiniteRunnerCharacter::GetPlayerScore()
{
	return PlayerScore;
}

int AInfiniteRunnerCharacter::GetCoinCount()
{
	return PlayerCoinCount;
}

float AInfiniteRunnerCharacter::GetEnergy()
{
	return Energy;
}
