// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"
/**
 * 
 */
UCLASS()
class PROJECT_HOPE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerHUD(const FObjectInitializer& ObjectInitializer);

public:
	// Health Bar
	void SetHealthBarFillPercent(float Percent);

	// Stamina Bar
	void SetStaminaBarFillPercent(float Percent);

	// Inventory
	void SetSlotItemImage(UTexture2D* Texture, FName TextureWidgetName);
	void SetSlotItemAmount(int ItemAmount, FName TextWidgetName);

	void SetWidgetOpacity(FName WidgetName, float Opacity);

	void SetChangeItemBarStatus(FName Status, FName SlotNumber);

	void SetAnimationStatus(FName Status, FString AnimationName);

	void ShowMessageOnScreen(FText Message);

	void UpdateWeaponHUD(struct FWeaponData WeaponData);

	UWidgetAnimation* GetWidgetAnimationFromName(FString AnimationName);

	template <typename T>
	void ShowCustomWidget(FName WidgetName, bool bShow);

	template <typename T>
	void ShowWidget(FName WidgetName, bool bShow);

	template <typename T>
	T* GetWidgetReference(FName WidgetName);

private:
	UWidgetAnimation* AnimationChangeItemMenuBar_Slot1;
	UWidgetAnimation* AnimationChangeItemMenuBar_Slot2;
	UWidgetAnimation* AnimationInteractMessage;
};

template<typename T>
inline void UPlayerHUD::ShowCustomWidget(FName WidgetName, bool bShow)
{
	if (T* Widget = dynamic_cast<T*>(GetWidgetFromName(WidgetName))){
		if (bShow) {
			Widget->SetColorAndOpacity(FLinearColor(0, 0, 0, 1.0f));
		}
		else {
			Widget->SetColorAndOpacity(FLinearColor(0, 0, 0, 0.0f));
		}
	}
}

template<typename T>
inline void UPlayerHUD::ShowWidget(FName WidgetName, bool bShow)
{
	if (T* Widget = dynamic_cast<T*>(GetWidgetFromName(WidgetName))) {
		if (bShow) {
			Widget->SetFillColorAndOpacity(FLinearColor(0, 0, 0, 1.0f));
		}
		else {
			Widget->SetFillColorAndOpacity(FLinearColor(0, 0, 0, 0.0f));
		}
	}
}

template<typename T>
inline T* UPlayerHUD::GetWidgetReference(FName WidgetName)
{
	if (T* Widget = dynamic_cast<T*>(GetWidgetFromName(WidgetName))) {
		return Widget;
	}

	return nullptr;
}
