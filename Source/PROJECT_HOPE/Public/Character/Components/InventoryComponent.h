// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class APickups;
class UEnhancedInputComponent;
class UInputAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_HOPE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AHCharacter;

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<APickups*> Inventory;

	TArray<APickups*> Items_Slot1;
	TArray<APickups*> Items_Slot2;

	bool bIsSlot1ChangeMenuOpen;
	bool bIsSlot2ChangeMenuOpen;

	FTimerHandle OpenChangeMenuProgressBar_Fill;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* UseSlot1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* OpenChangeSlot1ItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* OpenChangeSlot2ItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* ChangeNextSlotItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		UInputAction* ChangePreviousSlotItemAction;
public:

	void SetupInventoryInput(UEnhancedInputComponent* EnhancedInputComponent);

	void AddToInventory(APickups* Pickup);
	void RemoveFromInventory(APickups* Pickup);
	void RemoveFromInventory(FName PickupName);

private:

	void UseItemOnFirstSlot();

	void OpenSlot1ItemsMenu();
	void CloseSlot1ItemsMenu();

	void OpenSlot2ItemsMenu();
	void CloseSlot2ItemsMenu();

	void RefreshSlot(TArray<APickups*> &SlotArray, FName WidgetImage, FName WidgetCount);

	int GetItemCount(APickups* Pickup);

	void SortItems(TArray<APickups*>& Items_Array, APickups* Item);

	void Change_NextSlotItem();
	void Change_PreviousSlotItem();

	void StartOpenChangeSlot1ItemMenuProgressBar();
	void CancelOpenChangeSlot1ItemMenuProgressBar();
	void StartOpenChangeSlot2ItemMenuProgressBar();
	void CancelOpenChangeSlot2ItemMenuProgressBar();

	APickups* GetPickupFromName(FName PickupName);

public:
	TArray<APickups*> GetPlayerInventory();

	bool bHasItemInInventory(FName ItemName, TArray<APickups*> Array);
};
