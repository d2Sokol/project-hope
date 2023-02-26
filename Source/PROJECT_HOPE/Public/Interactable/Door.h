// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

class UBoxComponent;
class UWidgetComponent;
class UCurveFloat;

UENUM()
enum EDoorOpenDirection {
	FORWARD UMETA(DisplayName = "Forward"),
	BACKWARDS UMETA(DisplayName = "Backwards")
};

USTRUCT(BlueprintType)
struct FDoorInfo {

	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "DoorInfo")
		TEnumAsByte<EDoorOpenDirection> DoorOpenDirection;

	UPROPERTY(EditAnywhere, Category = "DoorInfo")
		FName ItemNeeded = "NONE";

	UPROPERTY(EditAnywhere, Category = "DoorInfo")
		bool bOpenedWithKey = false;
};

UCLASS()
class PROJECT_HOPE_API ADoor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ADoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* OpenableMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* FrameMesh;

	UPROPERTY(EditAnywhere)
		UBoxComponent* InteractBox;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* InteractHintWidget;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* InteractWidget;

	UPROPERTY(EditAnywhere)
		UCurveFloat* DoorCurve;

	bool bIsDoorOpen;
	bool bReadyState;

	float RotateValue;
	float CurveFloatValue;
	float TimelineValue;
	FRotator DoorRotation;
	FTimeline MyTimeline;

	FOnTimelineFloat TimelineCallback;
	FOnTimelineEventStatic TimelineFinishedCallback;

	UPROPERTY(EditAnywhere)
		FDoorInfo DoorInfo;
		

private:
	void SetupWidgets();

	virtual void ShowInteractHintWidget() override;
	virtual void HideInteractHintWidget() override;

	virtual void ShowInteractWidget() override;
	virtual void HideInteractWidget() override;

	void SetupDoorRotationValue();

	UFUNCTION()
	void ControlDoor();
	UFUNCTION()
	void ToogleDoor();
	UFUNCTION()
	void SetState();

public:
	virtual void Interact() override;

};
