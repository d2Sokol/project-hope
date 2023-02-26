// All rights reserved Life Before Studios ©


#include "Interactable/Pickups.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/HCharacter.h"
#include "Character/BasicCharacterNeeds.h"
#include "Character/Components/InventoryComponent.h"

// Sets default values
APickups::APickups()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));

	RootComponent = Mesh;
	//Mesh->SetupAttachment(RootComponent);
	HitSphere->SetupAttachment(RootComponent);

	HitSphere->SetSphereRadius(30.0f);

	InteractHintWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HintWidget"));
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));

	InteractHintWidget->SetupAttachment(RootComponent);
	InteractWidget->SetupAttachment(RootComponent);

	ItemData.PickupValue = 25.0f;
}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();
	
	SetupWidgets();

}

// Called every frame
void APickups::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickups::Interact()
{
	if (AHCharacter* Player = Cast<AHCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
		FString string = FString::Printf(TEXT("Picked Up %s" ), *ItemData.ItemDisplayName);
		ShowInteractMessage(Player, FText::FromString(string));
		Player->GetPlayerInventoryComponent()->AddToInventory(this);
		Destroy();
	}
}

void APickups::SetupWidgets()
{
	InteractHintWidget->SetVisibility(false);
	InteractWidget->SetVisibility(false);
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
	switch (ItemData.PickupType)
	{
	case MEDKIT:
		if (AHCharacter* Player = Cast<AHCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
			Player->GetBasicPlayerNeedsComponent()->AddPlayerHealth(ItemData.PickupValue);
		}
		break;
	case BANDAGE:
		if (AHCharacter* Player = Cast<AHCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
			Player->GetBasicPlayerNeedsComponent()->AddPlayerHealth(ItemData.PickupValue);
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

FItemData APickups::GetItemData()
{
	return ItemData;
}

UStaticMeshComponent* APickups::GetPickupMesh()
{
	return Mesh;
}


