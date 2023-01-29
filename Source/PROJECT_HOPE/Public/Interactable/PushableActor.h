// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Components/TimelineComponent.h"
#include "PushableActor.generated.h"

class UBoxComponent;
class UWidgetComponent;
class UArrowComponent;
class UCurveFloat;
class AHCharacter;

UCLASS()
class PROJECT_HOPE_API APushableActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	APushableActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		UBoxComponent* CollideBox;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PushableMesh;

	UPROPERTY(EditAnywhere)
		USceneComponent* AttachPlayerPosition;

	UPROPERTY(EditAnywhere)
		UBoxComponent* InteractBox;

	UPROPERTY(EditAnywhere)
		UArrowComponent* PushDirectionArrow;

	UPROPERTY(EditAnywhere)
		UCurveFloat* SetPlayerPosCurve;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* InteractHintWidget;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* InteractWidget;

	UPROPERTY(EditAnywhere, Category = "Animations")
		UAnimMontage* M_PushingAnim;

	UPROPERTY(EditAnywhere, Category = "Object Data")
		float ObjectWeight;

	bool bIsPushing;
	bool bCanPush;

	float CurveFloatValue;
	float TimelineValue;
	FTimeline MyTimeline;

	FOnTimelineFloat TimelineCallback;
	FOnTimelineEventStatic TimelineFinishedCallback;

	AHCharacter* Player;

private:
	void SetupWidgets();

	virtual void ShowInteractHintWidget() override;
	virtual void HideInteractHintWidget() override;

	virtual void ShowInteractWidget() override;
	virtual void HideInteractWidget() override;

	UFUNCTION()
	void SetPlayerPosSmoothly();

	UFUNCTION()
	void SetState();

	void TooglePushing();

	void UpdatePushableObjectPosition(float DeltaTime);

public:
	virtual void Interact() override;
};
