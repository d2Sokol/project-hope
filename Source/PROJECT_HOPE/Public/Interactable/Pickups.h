// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Pickups.generated.h"

class UBoxComponent;
class UWidgetComponent;

UENUM()
enum EPickupType {
	USABLE = 0 UMETA(DisplayName = "Usable"),
	MISSION = 1 UMETA(DisplayName = "Mission")
};

UENUM()
enum EUsablePickupType {
	MEDKIT = 0 UMETA(DisplayName = "Medkit"),
	BANDAGE UMETA(DisplayName = "Bandage"),
	ENERGY UMETA(DisplayName = "EnergyBoost"),
	DEAFULT
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FName ItemName = "";

};

UCLASS()
class PROJECT_HOPE_API APickups : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	APickups();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UBoxComponent* InteractDistanceBox;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* InteractHintWidget;

	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* InteractWidget;

	UPROPERTY(EditAnywhere, Category = "Item Type")
		TEnumAsByte<EUsablePickupType> UsablePickupType;

	UPROPERTY(EditAnywhere, Category = "Item Type")
		TEnumAsByte<EPickupType> PickupType;

	UPROPERTY(EditAnywhere, Category = "Image")
		UTexture2D* InventoryImage;

	UPROPERTY(EditAnywhere)
		FItemData ItemData;

	UPROPERTY(EditAnywhere, Category = "Pickup Value")
		float PickupValue;


private:

	virtual void Interact() override;

	void SetupWidgets();
	void SetupPickupValue();

	virtual void ShowInteractHintWidget() override;
	virtual void HideInteractHintWidget() override;

	virtual void ShowInteractWidget() override;
	virtual void HideInteractWidget() override;

protected:
	friend class UInventoryComponent;

	void Use();

public:
	UTexture2D* GetInventoryImage();

	EPickupType GetPickupType();

	FItemData GetItemData();
};
