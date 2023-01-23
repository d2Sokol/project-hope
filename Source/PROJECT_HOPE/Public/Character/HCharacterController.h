// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HCharacterController.generated.h"

/**
 * 
 */

class AHCharacter;

UCLASS()
class PROJECT_HOPE_API AHCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void OnPlayerDeath(AHCharacter* PlayerCharacter);

private:
	UPROPERTY(EditAnywhere, Category="Widgets")
		TSubclassOf<UUserWidget> DeathWidgetClass;
};
