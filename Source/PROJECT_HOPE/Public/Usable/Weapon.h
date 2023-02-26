// All rights reserved Life Before Studios ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interactable/Pickups.h"
#include "Weapon.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FName WeaponName;

	UPROPERTY(EditAnywhere)
		int32 WeaponAmmo;

	UPROPERTY(EditAnywhere)
		int32 WeaponMaxAmmo;

	UPROPERTY(EditAnywhere)
		float WeaponDamageMin;

	UPROPERTY(EditAnywhere)
		float WeaponDamageMax;

};

UCLASS()
class PROJECT_HOPE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
		USceneComponent* BulletSpawnPosition;

	UPROPERTY(EditAnywhere)
		FWeaponData WeaponData;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* FireParticle;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* HitParticle;

private:
	void InitWeaponData();

	void Shoot();

public:
	void PullTrigger();

	void Reload();
	const FWeaponData GetWeaponData() const;
};
