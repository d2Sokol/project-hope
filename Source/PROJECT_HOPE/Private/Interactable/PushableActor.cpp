// All rights reserved Life Before Studios ©


#include "Interactable/PushableActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Character/HCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APushableActor::APushableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollideBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollideBox"));
	RootComponent = CollideBox;

	PushableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PushableMesh"));
	PushableMesh->SetupAttachment(RootComponent);

	AttachPlayerPosition = CreateDefaultSubobject<USceneComponent>(TEXT("AttachPlayerPosition"));
	AttachPlayerPosition->SetupAttachment(PushableMesh);

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(PushableMesh);

	PushDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("PushDirectionArrow"));
	PushDirectionArrow->SetupAttachment(PushableMesh);

	InteractHintWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HintWidget"));
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));

	InteractHintWidget->SetupAttachment(PushableMesh);
	InteractWidget->SetupAttachment(PushableMesh);

	bIsPushing = false;
	bCanPush = false;

	ObjectWeight = 10.0f;
}

void APushableActor::BeginPlay()
{
	Super::BeginPlay();

	if (AHCharacter* PlayerRef = Cast<AHCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())) {
		Player = PlayerRef;
	}
	
	SetupWidgets();

	if (SetPlayerPosCurve) {
		TimelineCallback.BindUFunction(this, FName("SetPlayerPosSmoothly"));
		MyTimeline.AddInterpFloat(SetPlayerPosCurve, TimelineCallback);

		TimelineFinishedCallback.BindUFunction(this, FName("SetState"));
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void APushableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);

	if (bCanPush) {
		UpdatePushableObjectPosition(DeltaTime);
	}
}

void APushableActor::SetupWidgets()
{
	InteractHintWidget->SetVisibility(false);
	InteractWidget->SetVisibility(false);
}

void APushableActor::ShowInteractHintWidget()
{
	if (InteractHintWidget) {
		InteractHintWidget->SetVisibility(true);
	}
}

void APushableActor::HideInteractHintWidget()
{
	if (InteractHintWidget) {
		InteractHintWidget->SetVisibility(false);
	}
}

void APushableActor::ShowInteractWidget()
{
	if (InteractWidget) {
		InteractWidget->SetVisibility(true);
	}
}

void APushableActor::HideInteractWidget()
{
	if (InteractWidget) {
		InteractWidget->SetVisibility(false);
	}
}

void APushableActor::SetPlayerPosSmoothly()
{
	if (Player) {
		TimelineValue = MyTimeline.GetPlaybackPosition();
		CurveFloatValue = SetPlayerPosCurve->GetFloatValue(TimelineValue);
		FVector NewPlayerLocation = FMath::Lerp(Player->GetActorLocation(), AttachPlayerPosition->GetComponentLocation(), CurveFloatValue);
		FRotator NewPlayerRotation = FMath::Lerp(Player->GetControlRotation(), PushDirectionArrow->GetComponentRotation(), CurveFloatValue);
		Player->SetActorLocation(NewPlayerLocation);
		Player->GetController()->SetControlRotation(NewPlayerRotation);
		//Perhaps play pushing animation
	}
}

void APushableActor::SetState()
{
	bCanPush = true;
}

void APushableActor::TooglePushing()
{
	bIsPushing = !bIsPushing;
	bCanPush = false;
	if (Player) {
		if (bIsPushing) {
			Player->GetCharacterMovement()->MaxWalkSpeed = 1000.0f/ObjectWeight;
			if (Player->GetCharacterMovement()->MaxWalkSpeed > 200.0f) {
				Player->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
			}
			Player->SetCharacterTask(ECharacterTask::PUSHING_OBJECT);
			MyTimeline.PlayFromStart(); // Enable Timeline which trigger ControllPushableActor() On end bCanPush = true
		}
		else {
			Player->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
			Player->SetCharacterTask(ECharacterTask::NOTHING);
		}
	}
}

void APushableActor::UpdatePushableObjectPosition(float DeltaTime)
{
	FVector LocationOffset = Player->GetVelocity() * DeltaTime;


	AddActorLocalOffset(LocationOffset, true);

	if (!InteractWidget->IsVisible()) {
		TooglePushing();
	}
	
	//check collisions
}

void APushableActor::Interact()
{
	TooglePushing();
}

