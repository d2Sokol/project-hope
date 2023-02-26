// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

class UBoxComponent;
class IInteractInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_HOPE_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere)
		UBoxComponent* InteractBox;

	UPROPERTY(EditAnywhere)
		UBoxComponent* InteractHintBox;
		
	UBoxComponent* GetInteractBox();
	UBoxComponent* GetInteractHintBox();

	bool CanInteract();
	void Interact();

private:
	IInteractInterface* InteractInterface;
	TArray<IInteractInterface*> OverlappingHintInteract;
	bool bCanInteract;

private:
	void UpdateInteract();

	bool IsActorOnScreen(AActor* Actor);
};
