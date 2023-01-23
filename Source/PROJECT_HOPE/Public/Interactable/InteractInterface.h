// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_HOPE_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact();

	virtual void ShowInteractHintWidget();
	virtual void HideInteractHintWidget();

	virtual void ShowInteractWidget();
	virtual void HideInteractWidget();
};
