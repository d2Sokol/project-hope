// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Components/TimelineComponent.h"
#include "Chest.generated.h"

class UBoxComponent;
class UWidgetComponent;
class UCurveFloat;

UENUM()
enum EChestDropType {
	WEAPON_CHEST = 0,
	ITEMS_CHEST
};

USTRUCT()
struct FChestInfo {

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Chest Type")
		TEnumAsByte<EChestDropType> ChestDropType;

	UPROPERTY(EditAnywhere)
		bool RandomDrop = true;

	UPROPERTY(EditAnywhere, Category = "Items To Drop")
		TArray<TSubclassOf<class APickups>> ItemsDrop;

	UPROPERTY(EditAnywhere, Category = "DoorInfo")
		FName ItemNeeded = "NONE";

	UPROPERTY(EditAnywhere, Category = "DoorInfo")
		bool bOpenedWithKey = false;
};

USTRUCT()
struct FRandomItemDrop {

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "DropClass")
		TSubclassOf<class APickups> PickupClass;
	UPROPERTY(EditAnywhere, Category = "DropClass", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
		int32 DropRate;
};

UCLASS()
class PROJECT_HOPE_API AChest : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AChest();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* OpenableMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ChestMesh;

	UPROPERTY(EditAnywhere)
		UBoxComponent* InteractBox;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* InteractHintWidget;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* InteractWidget;

	UPROPERTY(EditAnywhere)
		USceneComponent* ItemsLocation;

	UPROPERTY(EditAnywhere)
		UCurveFloat* ChestCurve;

	UPROPERTY(EditAnywhere)
		FChestInfo ChestInfo;

	UPROPERTY(EditAnywhere)
		TArray<FRandomItemDrop> RandomItemsDrop;

	float CurveFloatValue;
	float TimelineValue;

	FTimeline MyTimeline;

	FOnTimelineFloat TimelineCallback;
	FOnTimelineEventStatic TimelineFinishedCallback;

	bool bIsChestOpen;
	bool bCanToogle;

	bool bItemsDropped;

private:
	void SetupWidgets();

	virtual void ShowInteractHintWidget() override;
	virtual void HideInteractHintWidget() override;

	virtual void ShowInteractWidget() override;
	virtual void HideInteractWidget() override;

	UFUNCTION()
		void ControlChest();
	UFUNCTION()
		void ToogleChest();
	UFUNCTION()
		void SetState();

	void DropItemsForPlayer();

	TSubclassOf<class APickups>& GetRandomItemClass();

public:
	virtual void Interact() override;
};
