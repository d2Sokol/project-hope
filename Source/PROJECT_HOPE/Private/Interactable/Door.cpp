// All rights reserved Life Before Studios ©


#include "Interactable/Door.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/HCharacter.h"
#include "Character/Components/InventoryComponent.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	OpenableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenableMesh"));
	
	FrameMesh->SetupAttachment(RootComponent);
	OpenableMesh->SetupAttachment(FrameMesh);

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(OpenableMesh);

	InteractHintWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HintWidget"));
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));

	InteractHintWidget->SetupAttachment(OpenableMesh);
	InteractWidget->SetupAttachment(OpenableMesh);

	bIsDoorOpen = false;
	bReadyState = true;

	DoorInfo.DoorOpenDirection = EDoorOpenDirection::FORWARD;
	DoorInfo.bOpenedWithKey = false;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	SetupWidgets();

	SetupDoorRotationValue();

	if (DoorCurve) {

		TimelineCallback.BindUFunction(this, FName("ControlDoor"));
		TimelineFinishedCallback.BindUFunction(this, FName("SetState"));
		MyTimeline.AddInterpFloat(DoorCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);
}

void ADoor::SetupWidgets()
{
	InteractHintWidget->SetVisibility(false);
	InteractWidget->SetVisibility(false);
}

void ADoor::ShowInteractHintWidget()
{
	if (InteractHintWidget) {
		InteractHintWidget->SetVisibility(true);
	}
}

void ADoor::HideInteractHintWidget()
{
	if (InteractHintWidget) {
		InteractHintWidget->SetVisibility(false);
	}
}

void ADoor::ShowInteractWidget()
{
	if (InteractWidget) {
		InteractWidget->SetVisibility(true);
	}
}

void ADoor::HideInteractWidget()
{
	if (InteractWidget) {
		InteractWidget->SetVisibility(false);
	}
}

void ADoor::SetupDoorRotationValue()
{
	switch (DoorInfo.DoorOpenDirection) {
		case FORWARD:
			RotateValue = -1.0f; break;
		case BACKWARDS:
			RotateValue = 1.0f; break;
	default:
		break;
	}
}

void ADoor::ControlDoor()
{
	TimelineValue = MyTimeline.GetPlaybackPosition();
	CurveFloatValue = RotateValue * DoorCurve->GetFloatValue(TimelineValue);

	FQuat NewRotation = FQuat(FRotator(0.0f, CurveFloatValue, 0.0f));
	OpenableMesh->SetRelativeRotation(NewRotation, true); // TODO Sweeping beetwen player and door
}

void ADoor::ToogleDoor()
{
	if (bReadyState) {
		bIsDoorOpen = !bIsDoorOpen;
		
		DoorRotation = OpenableMesh->GetRelativeRotation();
		
		if (bIsDoorOpen) {
			bReadyState = false;
			MyTimeline.PlayFromStart();
		}
		else {
			bReadyState = false;
			MyTimeline.Reverse();
		}
	}
}

void ADoor::SetState()
{
	bReadyState = true;
}

void ADoor::Interact()
{
	if (DoorInfo.ItemNeeded == "NONE") {
		ToogleDoor();
	}
	else {
		if (AHCharacter* Player = Cast<AHCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
			if (Player->GetPlayerInventoryComponent()->bHasItemInInventory(DoorInfo.ItemNeeded, Player->GetPlayerInventoryComponent()->GetPlayerInventory()) && !DoorInfo.bOpenedWithKey) {
				ShowInteractMessage(Player, FText::FromString("Used key to open door!"));
				ToogleDoor();
				DoorInfo.bOpenedWithKey = true;
				Player->GetPlayerInventoryComponent()->RemoveFromInventory(DoorInfo.ItemNeeded);
			}
			else if (DoorInfo.bOpenedWithKey) {
				ToogleDoor();
			}
			else {
				ShowInteractMessage(Player, FText::FromString("Door is locked!"));
				UE_LOG(LogTemp, Warning, TEXT("You need item!"));
			}
		}
	}
}

