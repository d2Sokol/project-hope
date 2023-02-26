// All rights reserved Life Before Studios ©


#include "Enemy/Enemy.h"
#include "Components/WidgetComponent.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyData.Health = 100.0f;
	EnemyData.MaxHealth = 100.0f;
	EnemyData.DamageMin = 25.0f;
	EnemyData.DamageMax = 30.0f;

	DamageAppliedWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageApplied"));
	DamageAppliedWidget->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	DamageAppliedWidget->SetVisibility(true);
	SetupUIAnimations();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::SetupUIAnimations()
{
	UWidgetBlueprintGeneratedClass* WidgetClass = DamageAppliedWidget->GetUserWidgetObject()->GetWidgetTreeOwningClass();
	if (WidgetClass) {
		for (UWidgetAnimation* WidgetAnimation : WidgetClass->Animations) {
			if (WidgetAnimation->GetName() == "ShowAppliedDamage_INST") {
				ShowDamageAppliedAnimation = Cast<UWidgetAnimation>(WidgetAnimation);
			}
		}
	}
}

void AEnemy::TakeDamageEnemy(float DamageValue)
{
	EnemyData.Health -= DamageValue;

	if (ShowDamageAppliedAnimation) {
		DamageAppliedWidget->GetUserWidgetObject()->PlayAnimation(ShowDamageAppliedAnimation);
	}

	if (UTextBlock* ItemAmountText = dynamic_cast<UTextBlock*>(DamageAppliedWidget->GetUserWidgetObject()->GetWidgetFromName("Damage"))) {
		int PrecDamage = (int)DamageValue;
		ItemAmountText->SetText(FText::AsNumber(PrecDamage));
	}
	
	if (EnemyData.Health <= 0.0f) {
		Destroy();
	}
}

FEnemyData& AEnemy::GetEnemyData()
{
	return EnemyData;
}

