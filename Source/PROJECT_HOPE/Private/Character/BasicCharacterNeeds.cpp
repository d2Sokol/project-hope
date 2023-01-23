// All rights reserved Life Before Studios ©


#include "Character/BasicCharacterNeeds.h"
#include "Character/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Character/HCharacter.h"

UBasicCharacterNeeds::UBasicCharacterNeeds()
{
	PrimaryComponentTick.bCanEverTick = false;

	Health = 100.0f;
	MaxHealth = 100.0f;

	Stamina = 100.0f;
	MaxStamina = 100.0f;

}

void UBasicCharacterNeeds::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBasicCharacterNeeds::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBasicCharacterNeeds::AddPlayerHealth(float IncreaseValue)
{

	if ((Health + IncreaseValue) > GetPlayerMaxHealth()) {
		SetPlayerHealth(GetPlayerMaxHealth());
	}
	else {
		Health += IncreaseValue;
	}

	if (PlayerHUD) {
		PlayerHUD->SetHealthBarFillPercent(Health/GetPlayerMaxHealth());
	}
}

void UBasicCharacterNeeds::DecreasePlayerHealth(float DecreaseValue)
{
	if ((Health - DecreaseValue) <= 0.0f) {
		Health = 0.0f;
		if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
			Player->Die();
		}
	}
	else {
		Health -= DecreaseValue;
	}

	if (PlayerHUD) {
		PlayerHUD->SetHealthBarFillPercent(Health / GetPlayerMaxHealth());
	}
}

void UBasicCharacterNeeds::AddPlayerStamina(float IncreaseValue)
{
	if ((Stamina + IncreaseValue) > GetPlayerMaxStamina()) {
		SetPlayerStamina(GetPlayerMaxStamina());
	}
	else {
		Stamina += IncreaseValue;
	}

	if (PlayerHUD) {
		PlayerHUD->SetStaminaBarFillPercent(Stamina / GetPlayerMaxStamina());
	}
}

void UBasicCharacterNeeds::DecreasePlayerStamina(float DecreaseValue)
{
	if ((Stamina - DecreaseValue) < 0.0f) {
		Stamina = 0.0f;
	}
	else {
		Stamina -= DecreaseValue;
	}

	if (PlayerHUD) {
		PlayerHUD->SetStaminaBarFillPercent(Stamina / GetPlayerMaxStamina());
	}
}

void UBasicCharacterNeeds::SetPlayerHealth(float HealthValue)
{
	Health = HealthValue;
}

void UBasicCharacterNeeds::SetPlayerStamina(float StaminaValue)
{
	Stamina = StaminaValue;
}

const float UBasicCharacterNeeds::GetPlayerHealth()
{
	return Health;
}

const float UBasicCharacterNeeds::GetPlayerMaxHealth()
{
	return MaxHealth;
}

const float UBasicCharacterNeeds::GetPlayerStamina()
{
	return Stamina;
}

const float UBasicCharacterNeeds::GetPlayerMaxStamina()
{
	return MaxStamina;
}

void UBasicCharacterNeeds::SetupPlayerHUD()
{
	if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
		PlayerHUD = Cast<UPlayerHUD>(Player->GetPlayerHUD());
	}

	PlayerHUD->SetHealthBarFillPercent(Health / MaxHealth);
	PlayerHUD->SetStaminaBarFillPercent(Stamina / MaxStamina);
}

