// All rights reserved Life Before Studios ©


#include "Interactable/Chest.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/HCharacter.h"
#include "Character/Components/InventoryComponent.h"
#include "Interactable/Pickups.h"

AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	OpenableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenableMesh"));

	ChestMesh->SetupAttachment(RootComponent);
	OpenableMesh->SetupAttachment(ChestMesh);

	ItemsLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ItemsLocation"));
	ItemsLocation->SetupAttachment(RootComponent);

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(OpenableMesh);

	InteractHintWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HintWidget"));
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));

	InteractHintWidget->SetupAttachment(OpenableMesh);
	InteractWidget->SetupAttachment(OpenableMesh);

	bIsChestOpen = false;
	bCanToogle = true;
	bItemsDropped = false;

	ChestInfo.ChestDropType = EChestDropType::ITEMS_CHEST;
	ChestInfo.ItemNeeded = "NONE";
}

void AChest::BeginPlay()
{
	Super::BeginPlay();
	
	SetupWidgets();

	if (ChestCurve) {

		TimelineCallback.BindUFunction(this, FName("ControlChest"));
		TimelineFinishedCallback.BindUFunction(this, FName("SetState"));
		MyTimeline.AddInterpFloat(ChestCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);
}

void AChest::SetupWidgets()
{
	InteractHintWidget->SetVisibility(false);
	InteractWidget->SetVisibility(false);
}

void AChest::ShowInteractHintWidget()
{
	if (InteractHintWidget) {
		InteractHintWidget->SetVisibility(true);
	}
}

void AChest::HideInteractHintWidget()
{
	if (InteractHintWidget) {
		InteractHintWidget->SetVisibility(false);
	}
}

void AChest::ShowInteractWidget()
{
	if (InteractWidget) {
		InteractWidget->SetVisibility(true);
	}
}

void AChest::HideInteractWidget()
{
	if (InteractWidget) {
		InteractWidget->SetVisibility(false);
	}
}

void AChest::ControlChest()
{
	TimelineValue = MyTimeline.GetPlaybackPosition();
	CurveFloatValue = -1.0f * ChestCurve->GetFloatValue(TimelineValue);

	FQuat NewRotation = FQuat(FRotator(0.f, 0.f, CurveFloatValue));
	OpenableMesh->SetRelativeRotation(NewRotation, true); // TODO Sweeping beetwen player and door
}

void AChest::ToogleChest()
{
	if (bCanToogle) {
		bIsChestOpen = !bIsChestOpen;

		if (bIsChestOpen) {
			bCanToogle = false;
			MyTimeline.PlayFromStart();
			if (!bItemsDropped) {
				DropItemsForPlayer();
			}
		}
		else {
			bCanToogle = false;
			MyTimeline.Reverse();
		}
	}
}

void AChest::SetState()
{
	bCanToogle = true;
}

void AChest::DropItemsForPlayer()
{
	bItemsDropped = true;
	if (ChestInfo.ChestDropType == EChestDropType::ITEMS_CHEST) {
		if (ChestInfo.RandomDrop == false) {
			for (int i = 0; i < ChestInfo.ItemsDrop.Num(); ++i) {
				int32 RandomX = FMath::RandRange(-30, 30);
				int32 RandomY = FMath::RandRange(-30, 30);
				APickups* Pickup = GetWorld()->SpawnActor<APickups>(ChestInfo.ItemsDrop[i]);
				Pickup->SetActorLocation(FVector(ItemsLocation->GetComponentLocation().X + RandomX, ItemsLocation->GetComponentLocation().Y + RandomY, ItemsLocation->GetComponentLocation().Z));
			}
		}
		else {
			int32 DropItemsCount = FMath::RandRange(1, 4);
			for (int i = 0; i < DropItemsCount; ++i) {
				int32 RandomX = FMath::RandRange(-30, 30);
				int32 RandomY = FMath::RandRange(-30, 30);
				APickups* Pickup = GetWorld()->SpawnActor<APickups>(GetRandomItemClass());
				Pickup->SetActorLocation(FVector(ItemsLocation->GetComponentLocation().X + RandomX, ItemsLocation->GetComponentLocation().Y + RandomY, ItemsLocation->GetComponentLocation().Z));
			}
		}
	}
}

TSubclassOf<class APickups>& AChest::GetRandomItemClass()
{
	TArray <int32> DropRateArray;

	for (int x = 0; x < RandomItemsDrop.Num(); ++x) {
		if (x == 0){
			DropRateArray.Add(FMath::Abs(RandomItemsDrop[x].DropRate));
		}
		else{
			DropRateArray.Add(FMath::Abs(RandomItemsDrop[x].DropRate) + (DropRateArray[x - 1]));
		}
	}

	if (DropRateArray.Last() == 100){
		int32 DropIndex = UKismetMathLibrary::RandomIntegerInRange(1, 100);

		for (int x = 0; x < DropRateArray.Num(); ++x){
			if (DropIndex <= DropRateArray[x]){

				return RandomItemsDrop[x].PickupClass;
				break;
			}
		}
	}

	return RandomItemsDrop[0].PickupClass;
}

void AChest::Interact()
{
	if (ChestInfo.ItemNeeded == "NONE") {
		ToogleChest();
	}
	else {
		if (AHCharacter* Player = Cast<AHCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
			if (Player->GetPlayerInventoryComponent()->bHasItemInInventory(ChestInfo.ItemNeeded, Player->GetPlayerInventoryComponent()->GetPlayerInventory()) && !ChestInfo.bOpenedWithKey) {
				ShowInteractMessage(Player, FText::FromString("Used key to open chest!"));
				ToogleChest();
				ChestInfo.bOpenedWithKey = true;
				Player->GetPlayerInventoryComponent()->RemoveFromInventory(ChestInfo.ItemNeeded);
			}
			else if (ChestInfo.bOpenedWithKey) {
				ToogleChest();
			}
			else {
				ShowInteractMessage(Player, FText::FromString("Chest is locked!"));
			}
		}
	}
}

