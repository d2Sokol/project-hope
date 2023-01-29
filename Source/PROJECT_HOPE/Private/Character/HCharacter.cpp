// All rights reserved Life Before Studios ©


#include "Character/HCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/BasicCharacterNeeds.h"
#include "Character/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Character/HCharacterController.h"
#include "Character/Components/InteractComponent.h"
#include "Components/BoxComponent.h"
#include "Character/Components/InventoryComponent.h"
#include "Character/Components/ClimbingComponent.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AHCharacter::AHCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh(), FName("Head"));

	BasicPlayerNeeds = CreateDefaultSubobject<UBasicCharacterNeeds>(TEXT("Character Needs"));

	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	InteractComponent->GetInteractBox()->SetupAttachment(GetMesh());
	InteractComponent->GetInteractHintBox()->SetupAttachment(GetMesh());

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	ClimbingComponent = CreateDefaultSubobject<UClimbingComponent>(TEXT("ClimbingComponent"));

	bIsRunning = false;
	bCanSprint = true;

	CharacterTask = ECharacterTask::NOTHING;
}

void AHCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetDefaultMappingContext(); // Set Input Context;
	SetDefaultActionsValues(); // Set Values to perform something e.g. jump stamina value
	
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	if (IsLocallyControlled() && HUDClass) {
		PlayerHUD = CreateWidget<UPlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0), HUDClass, FName("Player HUD"));
		PlayerHUD->AddToViewport();

		BasicPlayerNeeds->SetupPlayerHUD();
	}

	SetupTimers();

	CharacterState = ECharacterState::ALIVE;
}

void AHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTimers();
}

void AHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHCharacter::Look);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AHCharacter::Sprint);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHCharacter::StopSprint);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AHCharacter::TryJump);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AHCharacter::Interact);
		
		InventoryComponent->SetupInventoryInput(EnhancedInputComponent);
		ClimbingComponent->SetupClimbingInput(EnhancedInputComponent);
	}
}

void AHCharacter::Move(const FInputActionValue& ActionValue)
{
	if (!ClimbingComponent->bGetIsHunging()) {
		const FVector2D MovementVector = ActionValue.Get<FVector2D>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AHCharacter::Look(const FInputActionValue& ActionValue)
{
	if (!ClimbingComponent->bGetIsHunging()) {
		const FVector2D LookVector = ActionValue.Get<FVector2D>();
		AddControllerPitchInput(LookVector.Y * MouseYSensitivity);
		AddControllerYawInput(LookVector.X * MouseXSensitivity);
	}
}

void AHCharacter::Sprint()
{
	if (BasicPlayerNeeds->GetPlayerStamina() >= RunStaminaNeeded && !GetCharacterMovement()->IsFalling() && bCanSprint && CharacterTask == ECharacterTask::NOTHING) {
		if (!bIsRunning) {
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
			bIsRunning = true;
		}
	}
	else if(BasicPlayerNeeds->GetPlayerStamina() <= 0.0f && CharacterTask == ECharacterTask::NOTHING) {
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		bIsRunning = false;
	}
}

void AHCharacter::StopSprint()
{
	if (bIsRunning && CharacterTask == ECharacterTask::NOTHING) {
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}
	bIsRunning = false;
}

void AHCharacter::TryJump()
{
	if (BasicPlayerNeeds->GetPlayerStamina() >= JumpStaminaNeeded && !GetCharacterMovement()->IsFalling() && !GetCharacterMovement()->IsFlying() && CharacterTask != ECharacterTask::PUSHING_OBJECT) {
		if (ClimbingComponent->bGetCanHungAfterJump()) {
			Jump();
			BasicPlayerNeeds->DecreasePlayerStamina(JumpStaminaNeeded);
		}
		else if (ClimbingComponent->bGetCanClimbUp()) {
			ClimbingComponent->ClimbUp ();
		}
		else {
			Jump();
			BasicPlayerNeeds->DecreasePlayerStamina(JumpStaminaNeeded);
		}
	}
}

void AHCharacter::SetDefaultMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AHCharacter::SetDefaultActionsValues()
{
	JumpStaminaNeeded = 20.0f;
	RunStaminaNeeded = 10.0f;
	RunStaminaDecreaseValue = 1.0f;
	RunStaminaIncreaseValue = 1.0f;
}

void AHCharacter::SetupTimers()
{
	GetWorldTimerManager().SetTimer(StaminaDecreaseTimer, this, &AHCharacter::DecreaseStaminaTimerFunc, 0.1f, true);
	GetWorldTimerManager().PauseTimer(StaminaDecreaseTimer);

	GetWorldTimerManager().SetTimer(StaminaIncreaseTimer, this, &AHCharacter::IncreaseStaminaTimerFunc, 0.1f, true, 2.0f);
	GetWorldTimerManager().PauseTimer(StaminaIncreaseTimer);
}

void AHCharacter::IncreaseStaminaTimerFunc()
{
	BasicPlayerNeeds->AddPlayerStamina(RunStaminaDecreaseValue);
}

void AHCharacter::DecreaseStaminaTimerFunc()
{
	BasicPlayerNeeds->DecreasePlayerStamina(RunStaminaDecreaseValue);
}

void AHCharacter::UpdateTimers()
{
	//Decrease Stamina Timers
	if (bIsRunning) {
		if (GetWorldTimerManager().IsTimerPaused(StaminaDecreaseTimer)) {
			GetWorldTimerManager().UnPauseTimer(StaminaDecreaseTimer);
		}
	}
	else {
		if (!GetWorldTimerManager().IsTimerPaused(StaminaDecreaseTimer)) {
			GetWorldTimerManager().PauseTimer(StaminaDecreaseTimer);
		}
	}

	//Increase Stamina Timers
	if (!bIsRunning) {
		if (BasicPlayerNeeds->GetPlayerStamina() != BasicPlayerNeeds->GetPlayerMaxStamina()) {
			if (GetWorldTimerManager().IsTimerPaused(StaminaIncreaseTimer)) {
				GetWorldTimerManager().UnPauseTimer(StaminaIncreaseTimer);
			}
		}
		else {
			if (!GetWorldTimerManager().IsTimerPaused(StaminaIncreaseTimer)) {
				GetWorldTimerManager().PauseTimer(StaminaIncreaseTimer);
			}
		}
	}
	else {
		GetWorldTimerManager().PauseTimer(StaminaIncreaseTimer);
	}
}

void AHCharacter::Interact()
{
	if (InteractComponent->CanInteract()) {
		InteractComponent->Interact();
	}
}

UPlayerHUD* AHCharacter::GetPlayerHUD()
{
	return PlayerHUD;
}

const ECharacterState AHCharacter::GetCharacterStatus()
{
	return CharacterState;
}

UBasicCharacterNeeds* AHCharacter::GetBasicPlayerNeedsComponent()
{
	return BasicPlayerNeeds;
}

UInventoryComponent* AHCharacter::GetPlayerInventoryComponent()
{
	return InventoryComponent;
}

UCameraComponent* AHCharacter::GetCameraComponent()
{
	return Camera;
}

void AHCharacter::SetCharacterTask(ECharacterTask NewCharacterTask)
{
	CharacterTask = NewCharacterTask;
}

void AHCharacter::Die()
{
	CharacterState = ECharacterState::DEAD;
	bIsRunning = false;

	if (AHCharacterController* PlayerController = Cast<AHCharacterController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(DefaultMappingContext);
		}
		PlayerController->OnPlayerDeath(this);
	}
}


