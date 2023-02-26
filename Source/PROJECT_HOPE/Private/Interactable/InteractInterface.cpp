// All rights reserved Life Before Studios ©


#include "Interactable/InteractInterface.h"
#include "Character/HCharacter.h"
#include "Character/PlayerHUD.h"

// Add default functionality here for any IInteractInterface functions that are not pure virtual.

void IInteractInterface::Interact()
{

}

void IInteractInterface::ShowInteractHintWidget()
{
}

void IInteractInterface::HideInteractHintWidget()
{
}

void IInteractInterface::ShowInteractWidget()
{
}

void IInteractInterface::HideInteractWidget()
{
}

void IInteractInterface::ShowInteractMessage(AHCharacter* Player, FText Message)
{
	if (Player) {
		Player->GetPlayerHUD()->ShowMessageOnScreen(Message);
	}
}
