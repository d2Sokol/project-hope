// All rights reserved Life Before Studios ©


#include "Character/PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UPlayerHUD::UPlayerHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

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

//template<typename T>
//T* UPlayerHUD::GetWidgetReference(FName WidgetName)
//{
//	if (T* WidgetReference = dynamic_cast<T*>(GetWidgetFromName(WidgetName))) {
//		return WidgetReference;
//	}
//
//	return nullptr;
//}

