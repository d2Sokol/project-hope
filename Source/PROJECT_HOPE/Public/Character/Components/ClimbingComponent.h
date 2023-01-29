// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClimbingComponent.generated.h"

class UEnhancedInputComponent;
class UInputAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_HOPE_API UClimbingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UClimbingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* HungToClimbUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* HungToCancel;

	UPROPERTY(EditAnywhere, Category = "Animations")
		UAnimMontage* M_Hung;

	UPROPERTY(EditAnywhere, Category = "Animations")
		UAnimMontage* M_HungToClimb;

	UPROPERTY(EditAnywhere, Category = "Animations")
		UAnimMontage* M_HungToDrop;

private:
	FVector HeightLocation;
	FVector WallNormal;
	FVector WallLocation;

private:
	void TraceForward();
	void HeightTrace();

	void CancelHunging();
	
	void CheckIfIsGrounded();
private:
	bool bCanHung;
	bool bCanHungAfterJump;
	bool bCanClimbUp;
	bool bIsHunging;
	bool bCheckForFloorContact;
public:
	void SetupClimbingInput(UEnhancedInputComponent* EnhancedInputComponent);

	void ClimbUp();

	void Hung();

	const bool bGetCanClimbUp();
	const bool bGetCanHungAfterJump();
	const bool bGetCanHung();
	const bool bGetIsHunging();

	UFUNCTION(BlueprintCallable)
		void OnClimbAnimationNotify();
};
