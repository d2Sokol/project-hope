// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UBasicCharacterNeeds;
class UUserWidget;
class UPlayerHUD;
class UInteractComponent;
class UInventoryComponent;
class UClimbingComponent;

UENUM()
enum ECharacterState : int {
	ALIVE = 0,
	DEAD = 1
};

enum ECharacterTask : int {
	PUSHING_OBJECT = 0,
	NOTHING
};

UCLASS()
class PROJECT_HOPE_API AHCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Mouse Values")
		float MouseYSensitivity = 0.7f;

	UPROPERTY(EditAnywhere, Category = "Mouse Values")
		float MouseXSensitivity = 0.6f;

protected:

	void Move(const FInputActionValue& ActionValue);

	void Look(const FInputActionValue& ActionValue);

	void Sprint();

	void StopSprint();

	void TryJump();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Components")
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "Components")
		UBasicCharacterNeeds* BasicPlayerNeeds;

	UPROPERTY(EditAnywhere, Category = "Components")
		UInteractComponent* InteractComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
		UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere)
		UPlayerHUD* PlayerHUD;

	UPROPERTY(EditAnywhere)
		FTimerHandle StaminaIncreaseTimer;

	UPROPERTY(EditAnywhere)
		FTimerHandle StaminaDecreaseTimer;

	ECharacterState CharacterState;
	ECharacterTask CharacterTask;

	UPROPERTY(EditAnywhere, Category = "Actions Values")
		float JumpStaminaNeeded;

	UPROPERTY(EditAnywhere, Category = "Actions Values")
		float RunStaminaNeeded;
	
	UPROPERTY(EditAnywhere, Category = "Actions Values")
		float RunStaminaIncreaseValue;

	UPROPERTY(EditAnywhere, Category = "Actions Values")
		float RunStaminaDecreaseValue;

	UPROPERTY(EditAnywhere, Category = "Movement States")
		bool bIsRunning;

private:
	void SetDefaultMappingContext();

	void SetDefaultActionsValues();

	void SetupTimers();

	void IncreaseStaminaTimerFunc();
	void DecreaseStaminaTimerFunc();

	void UpdateTimers();

	void Interact();

public:

	UPROPERTY(EditAnywhere, Category = "Movement States")
		bool bCanSprint;

public:
	UPlayerHUD* GetPlayerHUD();

	const ECharacterState GetCharacterStatus();

	UBasicCharacterNeeds* GetBasicPlayerNeedsComponent();

	UInventoryComponent* GetPlayerInventoryComponent();

	UCameraComponent* GetCameraComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UClimbingComponent* ClimbingComponent;

public:
	void SetCharacterTask(ECharacterTask NewCharacterTask);

	void Die();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayAnimationBlueprint();
};
