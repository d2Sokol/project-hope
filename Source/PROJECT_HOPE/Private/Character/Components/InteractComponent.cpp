// All rights reserved Life Before Studios ©


#include "Character/Components/InteractComponent.h"
#include "Components/BoxComponent.h"
#include "Character/HCharacter.h"
#include "Interactable/Pickups.h"
#include "Interactable/InteractInterface.h"
#include "Character/Components/InventoryComponent.h"
#include "GameFramework/PlayerController.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractComponent | InteractBox"));
	InteractHintBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractComponent | InteractHintBox"));
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	bCanInteract = false;

}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateInteract();
}

UBoxComponent* UInteractComponent::GetInteractBox()
{
	return InteractBox;
}

UBoxComponent* UInteractComponent::GetInteractHintBox()
{
	return InteractHintBox;
}

bool UInteractComponent::CanInteract()
{
	return InteractInterface != nullptr;
}

void UInteractComponent::Interact()
{
	if (InteractInterface) {
		InteractInterface->Interact();
	}
}

void UInteractComponent::UpdateInteract()
{

	TArray<AActor*> OverlappingHintActors;
	InteractHintBox->GetOverlappingActors(OverlappingHintActors);

	//Hide old hint
	if (!OverlappingHintInteract.IsEmpty()) {
		for (IInteractInterface* WidgetToHide : OverlappingHintInteract) {
			WidgetToHide->HideInteractHintWidget();
			WidgetToHide = nullptr;
		}
		OverlappingHintInteract.Empty(); //Clear all hint widgets
	}

	//Show hint
	if (OverlappingHintActors.Num() > 0) { // Check if is overlapping with something.
		for (AActor* OverlappingActor : OverlappingHintActors) {
			if (IInteractInterface* FoundInteract = Cast<IInteractInterface>(OverlappingActor)) {
				FoundInteract->ShowInteractHintWidget();
				OverlappingHintInteract.Push(FoundInteract); //Add new hint to array
			}
		}
	}

	TArray<AActor*> OverlappingInteractActors;
	InteractBox->GetOverlappingActors(OverlappingInteractActors);

	if (OverlappingInteractActors.IsEmpty()) { //If there is no actors that player can interact with
		if (InteractInterface) { //Hide last interact widget
			InteractInterface->HideInteractWidget();
			InteractInterface = nullptr;
		}//
	}

	
	if (OverlappingInteractActors.Num() > 0) {
		if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
			AActor* ClosestActor = OverlappingInteractActors[0];
			for (AActor* OverlappingActor : OverlappingInteractActors) {
				if (AActor* InteractActor = Cast<AActor>(OverlappingActor)) {
					if (Player->GetDistanceTo(InteractActor) < Player->GetDistanceTo(ClosestActor)) { //Find closest interact actor
						ClosestActor = InteractActor;
					}
				}
			}
			if (IInteractInterface* InteractActor = Cast<IInteractInterface>(ClosestActor)) {
				if (InteractInterface) {
					InteractInterface->HideInteractWidget();
					InteractInterface = nullptr;
				}
				InteractInterface = InteractActor;
				InteractInterface->HideInteractHintWidget();
				InteractInterface->ShowInteractWidget();
			}
		}
	}
}

bool UInteractComponent::IsActorOnScreen(AActor* Actor)
{
	bool bIsOnScreen = false;
	if (Actor == nullptr) return false;

	APawn* PlayerPawn = Cast<APawn>(GetOwner());
	if (PlayerPawn == nullptr) return false;

	const APlayerController* const PlayerController = Cast<const APlayerController>(PlayerPawn->GetController());
	if (PlayerController == nullptr) return false;

	FVector2D ScreenLocation;
	PlayerController->ProjectWorldLocationToScreen(Actor->GetActorLocation(), ScreenLocation);

	int32 ScreenWidth = 0;
	int32 ScreenHeight = 0;
	PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

	int32 ScreenX = (int32)ScreenLocation.X;
	int32 ScreenY = (int32)ScreenLocation.Y;
	
	if (ScreenX >= 0 && ScreenY >= 0 && ScreenX < ScreenWidth && ScreenY < ScreenHeight) {
		bIsOnScreen = true;
	}
	else {
		bIsOnScreen = false;
	}

	return bIsOnScreen;
}

