// All rights reserved Life Before Studios ©


#include "Character/HCharacterController.h"
#include "Character/HCharacter.h"
#include "Blueprint/UserWidget.h"

void AHCharacterController::OnPlayerDeath(AHCharacter* PlayerCharacter)
{
	if (DeathWidgetClass) {
		UUserWidget* DeathWidget = CreateWidget(this, DeathWidgetClass);
		DeathWidget->AddToViewport();
	}
}
