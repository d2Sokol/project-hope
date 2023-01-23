// All rights reserved Life Before Studios ©


#include "Interactable/Pickups.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/HCharacter.h"
#include "Character/BasicCharacterNeeds.h"
#include "Character/Components/InventoryComponent.h"

// Sets default values
APickups::APickups()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	InteractDistanceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractDistanceBox"));

	Mesh->SetupAttachment(RootComponent);
	InteractDistanceBox->SetupAttachment(Mesh);

	InteractDistanceBox->SetBoxExtent(FVector(64.0f, 64.0f, 32.0f));

	InteractHintWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HintWidget"));
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));

	InteractHintWidget->SetupAttachment(RootComponent);
	InteractWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();
	
	SetupWidgets();
	SetupPickupValue();

}

// Called every frame
void APickups::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickups::Interact()
{
	if (AHCharacter* Player = Cast<AHCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
		Player->GetPlayerInventoryComponent()->AddToInventory(this);
		Destroy();
	}
}

void APickups::SetupWidgets()
{
	InteractHintWidget->SetVisibility(false);
	InteractWidget->SetVisibility(false);
}

void APickups::SetupPickupValue()
{
	PickupValue = 25.0f;
}

void APickups::ShowInteractHintWidget()
{
	if (InteractHintWidget) {
		InteractHintWidget->SetVisibility(true);
	}
}

void APickups::HideInteractHintWidget()
{
	if (InteractHintWidget) {
		InteractHintWidget->SetVisibility(false);
	}
}

void APickups::ShowInteractWidget()
{
	if (InteractWidget) {
		InteractWidget->SetVisibility(true);
	}
}

void APickups::HideInteractWidget()
{
	if (InteractWidget) {
		InteractWidget->SetVisibility(false);
	}
}

void APickups::Use()
{
	switch (PickupType)
	{
	case MEDKIT:
		if (AHCharacter* Player = Cast<AHCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
			Player->GetBasicPlayerNeedsComponent()->AddPlayerHealth(PickupValue);
		}
		break;
	case BANDAGE:
		if (AHCharacter* Player = Cast<AHCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
			Player->GetBasicPlayerNeedsComponent()->AddPlayerHealth(PickupValue);
		}
		break;
	case ENERGY:
		if (AHCharacter* Player = Cast<AHCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
			//Perhaps decrease stamina usage for 20sec or something
		}
		break;
	default:
		break;
	}
}

UTexture2D* APickups::GetInventoryImage()
{
	return InventoryImage;
}

EPickupType APickups::GetPickupType()
{
	return PickupType;
}

FItemData APickups::GetItemData()
{
	return ItemData;
}


