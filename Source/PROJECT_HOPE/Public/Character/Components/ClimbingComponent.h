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
	UPROPERTY(EditAnywhere, Category = "Animations")
		UAnimMontage* M_HungToClimb;

private:
	void HeightTrace();
	void ClimbUp();

private:
	bool bCanClimbUpAfterJump;
	bool bCanClimbUp;

public:
	UFUNCTION(BlueprintCallable)
		void OnClimbAnimationNotify();
};
