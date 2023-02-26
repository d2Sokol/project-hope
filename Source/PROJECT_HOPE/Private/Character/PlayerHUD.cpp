// All rights reserved Life Before Studios ©


#include "Character/PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Usable/Weapon.h"

UPlayerHUD::UPlayerHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AnimationChangeItemMenuBar_Slot1 = Cast<UWidgetAnimation>(GetWidgetAnimationFromName("Slot1_RadialProgressBarAnimation_INST"));
	AnimationChangeItemMenuBar_Slot2 = Cast<UWidgetAnimation>(GetWidgetAnimationFromName("Slot2_RadialProgressBarAnimation_INST"));
	AnimationInteractMessage = Cast<UWidgetAnimation>(GetWidgetAnimationFromName("InteractMessageAnimation_INST"));
}

void UPlayerHUD::SetHealthBarFillPercent(float Percent)
{
	if (UProgressBar* HealthBar = dynamic_cast<UProgressBar*>(GetWidgetFromName(FName("HealthBar")))) {
		HealthBar->SetPercent(Percent);
	}
}

void UPlayerHUD::SetStaminaBarFillPercent(float Percent)
{
	if (UProgressBar* StaminaBar = dynamic_cast<UProgressBar*>(GetWidgetFromName(FName("StaminaBar")))) {
		StaminaBar->SetPercent(Percent);
	}

}

void UPlayerHUD::SetSlotItemImage(UTexture2D* Texture, FName TextureWidgetName)
{
	if (UImage* InvTexture = dynamic_cast<UImage*>(GetWidgetFromName(TextureWidgetName))) {
		InvTexture->SetBrushFromTexture(Texture);
		if (Texture != nullptr) {
			InvTexture->SetOpacity(1.0f);
		}
		else {
			InvTexture->SetOpacity(0.0f);
		}
	}
}

void UPlayerHUD::SetSlotItemAmount(int ItemAmount, FName TextWidgetName)
{
	if (UTextBlock* ItemAmountText = dynamic_cast<UTextBlock*>(GetWidgetFromName(TextWidgetName))) {
		ItemAmountText->SetText(FText::AsNumber(ItemAmount));
		if (ItemAmount != 0) {
			ItemAmountText->SetOpacity(1.0f);
		}
		else {
			ItemAmountText->SetOpacity(0.0f);
		}
	}
}

void UPlayerHUD::SetWidgetOpacity(FName WidgetName, float Opacity)
{
	if (UImage* WidgetImage = dynamic_cast<UImage*>(GetWidgetFromName(WidgetName))) {
		WidgetImage->SetOpacity(Opacity);
	}
}

void UPlayerHUD::SetChangeItemBarStatus(FName Status, FName SlotNumber)
{
	UWidgetAnimation* Animation = nullptr;
	if (SlotNumber == "SLOT_1") {
		Animation = AnimationChangeItemMenuBar_Slot1;
	}
	else if (SlotNumber == "SLOT_2") {
		Animation = AnimationChangeItemMenuBar_Slot2;
	}

	if (Status == "START") {
		PlayAnimation(Animation);
	}
	else if (Status == "STOP") {
		StopAnimation(Animation);
	}
}

void UPlayerHUD::SetAnimationStatus(FName Status, FString AnimationName)
{
	if (UWidgetAnimation* Animation = Cast<UWidgetAnimation>(GetWidgetAnimationFromName(AnimationName))) {
		if (Status == "START") {
			PlayAnimation(Animation);
		}
		else if (Status == "STOP") {
			StopAnimation(Animation);
		}
	}
}

void UPlayerHUD::ShowMessageOnScreen(FText Message)
{
	if (UTextBlock* ScreenMessageText = dynamic_cast<UTextBlock*>(GetWidgetFromName("InteractMessage"))) {
		ScreenMessageText->SetText(Message);
		PlayAnimation(AnimationInteractMessage);
	}
}

void UPlayerHUD::UpdateWeaponHUD(FWeaponData WeaponData)
{
	UUserWidget* WeaponWidget = GetWidgetReference<UUserWidget>("BP_WeaponWidget");
	if (WeaponWidget == nullptr) return;

	if (UTextBlock* AmmoText = dynamic_cast<UTextBlock*>(WeaponWidget->GetWidgetFromName("Ammo"))) {
		AmmoText->SetText(FText::AsNumber(WeaponData.WeaponAmmo));
	}

	if (UTextBlock* MaxAmmoText = dynamic_cast<UTextBlock*>(WeaponWidget->GetWidgetFromName("MaxAmmo"))) {
		MaxAmmoText->SetText(FText::AsNumber(WeaponData.WeaponMaxAmmo));
	}
}

UWidgetAnimation* UPlayerHUD::GetWidgetAnimationFromName(FString AnimationName)
{
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	if (WidgetClass != nullptr) {
		for (UWidgetAnimation* WidgetAnimation : WidgetClass->Animations) {
			if (WidgetAnimation->GetName() == AnimationName) {
				return WidgetAnimation;
			}
		}
	}

	return nullptr;
}

