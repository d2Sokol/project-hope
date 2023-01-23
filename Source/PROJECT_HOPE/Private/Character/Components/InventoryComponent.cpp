// All rights reserved Life Before Studios ©


#include "Character/Components/InventoryComponent.h"
#include "Interactable/Pickups.h"
#include "Character/HCharacter.h"
#include "Character/PlayerHUD.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	bIsSlot1ChangeMenuOpen = false;
	bIsSlot2ChangeMenuOpen = false;
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::SetupInventoryInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(UseSlot1Action, ETriggerEvent::Completed, this, &UInventoryComponent::UseItemOnFirstSlot);

	//EnhancedInputComponent->BindAction(OpenChangeSlot1ItemAction, ETriggerEvent::Started, this, &AHCharacter::UseInventorySlot1); -- Start progress bar
	//EnhancedInputComponent->BindAction(OpenChangeSlot1ItemAction, ETriggerEvent::Canceled, this, &AHCharacter::UseInventorySlot1); -- Stop progress bar
	EnhancedInputComponent->BindAction(OpenChangeSlot1ItemAction, ETriggerEvent::Triggered, this, &UInventoryComponent::OpenSlot1ItemsMenu); //Open change menu
	EnhancedInputComponent->BindAction(OpenChangeSlot1ItemAction, ETriggerEvent::Completed, this, &UInventoryComponent::CloseSlot1ItemsMenu); //Close change menu

	EnhancedInputComponent->BindAction(ChangeSlot1ItemAction, ETriggerEvent::Started, this, &UInventoryComponent::ChangeSlot1Item);

}

void UInventoryComponent::AddToInventory(APickups* Pickup)
{
	if (Pickup) {
		Inventory.Push(Pickup);
		if (Pickup->GetPickupType() == EPickupType::USABLE) {
			if (!bHasItemInInventory(Pickup->GetItemData().ItemName, Items_Slot1)) {
				Items_Slot1.Insert(Pickup, 0);
			}
			RefreshSlot(Items_Slot1, "ACTIVE_SLOT_1_ITEM_IMAGE", "ACTIVE_SLOT_1_ITEM_COUNT");
		}
		else {
			if (!bHasItemInInventory(Pickup->GetItemData().ItemName, Items_Slot2)) {
				Items_Slot2.Insert(Pickup, 0);
			}
			RefreshSlot(Items_Slot2, "ACTIVE_SLOT_2_ITEM_IMAGE", "ACTIVE_SLOT_2_ITEM_COUNT");
		}
	}
}

void UInventoryComponent::RemoveFromInventory(APickups* Pickup)
{
	if (Pickup) {
		Inventory.Remove(Pickup);
		if (Pickup->GetPickupType() == EPickupType::USABLE) {
			Items_Slot1.Remove(Pickup);
			if (bHasItemInInventory(Pickup->GetItemData().ItemName, Inventory)) {
				for (APickups* InventoryPickup : Inventory) {
					if (InventoryPickup->GetItemData().ItemName == Pickup->GetItemData().ItemName) {
						Items_Slot1.Insert(InventoryPickup, 0);
					}
				}
			}
			RefreshSlot(Items_Slot1, "ACTIVE_SLOT_1_ITEM_IMAGE", "ACTIVE_SLOT_1_ITEM_COUNT");
		}
		else {
			Items_Slot2.Remove(Pickup);
			RefreshSlot(Items_Slot2, "ACTIVE_SLOT_2_ITEM_IMAGE", "ACTIVE_SLOT_2_ITEM_COUNT");
		}
	}
}

void UInventoryComponent::UseItemOnFirstSlot()
{
	if (!Items_Slot1.IsEmpty()) {
		if (APickups* ActivePickup = Cast<APickups>(Items_Slot1[0])) {
			ActivePickup->Use();
			RemoveFromInventory(ActivePickup);
		}
	}
}

void UInventoryComponent::OpenSlot1ItemsMenu()
{
	if (!bIsSlot1ChangeMenuOpen) {
		bIsSlot1ChangeMenuOpen = true;
		if (!Items_Slot1.IsEmpty()) {
			if (APickups* ActivePickup = Cast<APickups>(Items_Slot1[0])) {
				if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
					Player->GetPlayerHUD()->SetWidgetOpacity("INV_SLOT_1_CHANGE", 1.0f);
					Player->GetPlayerHUD()->SetSlotItemImage(ActivePickup->GetInventoryImage(), "ACTIVE_SLOT_1_ITEM_IMAGE_CHANGE");
					Player->GetPlayerHUD()->SetSlotItemAmount(GetItemCount(ActivePickup), "ACTIVE_SLOT_1_ITEM_COUNT_CHANGE");
				}
			}
		}
		else {
			if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
				Player->GetPlayerHUD()->SetWidgetOpacity("INV_SLOT_1_CHANGE", 1.0f);
			}
		}
	}
}

void UInventoryComponent::CloseSlot1ItemsMenu()
{
	if (bIsSlot1ChangeMenuOpen) {
		if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
			bIsSlot1ChangeMenuOpen = false;
			Player->GetPlayerHUD()->SetWidgetOpacity("INV_SLOT_1_CHANGE", 0.0f);

			Player->GetPlayerHUD()->SetSlotItemImage(nullptr, "ACTIVE_SLOT_1_ITEM_IMAGE_CHANGE");
			Player->GetPlayerHUD()->SetSlotItemAmount(0, "ACTIVE_SLOT_1_ITEM_COUNT_CHANGE");
			RefreshSlot(Items_Slot1, "ACTIVE_SLOT_1_ITEM_IMAGE", "ACTIVE_SLOT_1_ITEM_COUNT");
		}
	}
}

void UInventoryComponent::RefreshSlot(TArray<APickups*> &SlotArray, FName WidgetImage, FName WidgetCount)
{
	if (!SlotArray.IsEmpty()) {
		APickups* ActivePickup = Cast<APickups>(SlotArray[0]);

		if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
			Player->GetPlayerHUD()->SetSlotItemImage(ActivePickup->GetInventoryImage(), WidgetImage);
			Player->GetPlayerHUD()->SetSlotItemAmount(GetItemCount(ActivePickup), WidgetCount);
			if (GetItemCount(ActivePickup) >= 2) {
				SortItems(SlotArray, ActivePickup);
			}
		}
	}
	else {
		if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
			Player->GetPlayerHUD()->SetSlotItemImage(nullptr, WidgetImage);
			Player->GetPlayerHUD()->SetSlotItemAmount(0, WidgetCount);
		}
	}
}


int UInventoryComponent::GetItemCount(APickups* Pickup)
{
	int Count = 0;

	for (APickups* CurrentPickup : Inventory) {
		if (CurrentPickup->GetItemData().ItemName == Pickup->GetItemData().ItemName) {
			Count++;
		}
	}

	return Count;
}

void UInventoryComponent::SortItems(TArray<APickups*>& Items_Array, APickups* Item)
{
	TArray<APickups*> ArrayOfActiveItems;
	for (APickups* Pickup : Items_Array) {
		if (Pickup->GetItemData().ItemName == Item->GetItemData().ItemName) {
			ArrayOfActiveItems.Add(Pickup);
		}
	}
	for (APickups* Pickup : ArrayOfActiveItems) {
		Items_Array.Remove(Pickup);
		Items_Array.Insert(Pickup, 0);
	}
}

void UInventoryComponent::ChangeSlot1Item()
{
	if (!Items_Slot1.IsEmpty()) {
		APickups* ActivePickup = Items_Slot1[0];
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActivePickup->GetName());
		Items_Slot1.Remove(ActivePickup); //from index 0
		Items_Slot1.Add(ActivePickup); // to last index

		bIsSlot1ChangeMenuOpen = false;
		OpenSlot1ItemsMenu();

	}
}

TArray<APickups*> UInventoryComponent::GetPlayerInventory()
{
	return Inventory;
}

bool UInventoryComponent::bHasItemInInventory(FName ItemName, TArray<APickups*> Array)
{
	for (APickups* Item : Array) {
		if (Item->GetItemData().ItemName == ItemName) {
			return true;
		}
	}

	return false;
}
