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

	//template <typename T> 
	//T* GetWidgetReference(FName WidgetName);
};
