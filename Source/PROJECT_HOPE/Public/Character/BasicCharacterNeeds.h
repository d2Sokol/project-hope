// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicCharacterNeeds.generated.h"

class UPlayerHUD;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_HOPE_API UBasicCharacterNeeds : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBasicCharacterNeeds();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPlayerHUD* PlayerHUD;

private:
	UPROPERTY(EditAnywhere)
		float Health;

	UPROPERTY(EditAnywhere)
		float MaxHealth;

	UPROPERTY(EditAnywhere)
		float Stamina;

	UPROPERTY(EditAnywhere)
		float MaxStamina;

public:
	void AddPlayerHealth(float IncreaseValue);
	
	void DecreasePlayerHealth(float DecreaseValue);

	void AddPlayerStamina(float IncreaseValue);

	void DecreasePlayerStamina(float DecreaseValue);

	void SetPlayerHealth(float HealthValue);

	void SetPlayerStamina(float StaminaValue);

	UFUNCTION(BlueprintCallable)
		const float GetPlayerHealth();

	UFUNCTION(BlueprintCallable)
		const float GetPlayerMaxHealth();

	UFUNCTION(BlueprintCallable)
		const float GetPlayerStamina();

	UFUNCTION(BlueprintCallable)
		const float GetPlayerMaxStamina();

	void SetupPlayerHUD();
};
