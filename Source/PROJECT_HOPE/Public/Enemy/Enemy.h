// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UWidgetComponent;

USTRUCT(BlueprintType)
struct FEnemyData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "EnemyInfo")
		float Health;

	UPROPERTY(EditAnywhere, Category = "EnemyInfo")
		float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "EnemyInfo")
		float DamageMin;

	UPROPERTY(EditAnywhere, Category = "EnemyInfo")
		float DamageMax;

};

UCLASS()
class PROJECT_HOPE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere)
		FEnemyData EnemyData;


	UPROPERTY(EditAnywhere, Category = "Widgets")
		UWidgetComponent* DamageAppliedWidget;

	class UWidgetAnimation* ShowDamageAppliedAnimation;

private:
	void SetupUIAnimations();

public:
	void TakeDamageEnemy(float DamageValue);

	FEnemyData& GetEnemyData();

};
