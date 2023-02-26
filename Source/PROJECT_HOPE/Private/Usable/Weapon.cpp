// All rights reserved Life Before Studios ©


#include "Usable/Weapon.h"
#include "Components/SceneComponent.h"
#include "Character/HCharacter.h"
#include "Character/BasicCharacterNeeds.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/PlayerHUD.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	WeaponMesh->SetupAttachment(Root);

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));
	BulletSpawnPosition->SetupAttachment(WeaponMesh);

	InitWeaponData();
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::InitWeaponData()
{
	WeaponData.WeaponName = "WEAPON_DEFAULT";
	WeaponData.WeaponAmmo = 16;
	WeaponData.WeaponMaxAmmo = 16;
	WeaponData.WeaponDamageMin = 25.0f;
	WeaponData.WeaponDamageMax = 30.0f;
}

void AWeapon::Shoot()
{
	APawn* PlayerPawn = Cast<APawn>(GetOwner());
	if (PlayerPawn == nullptr) return;

	AController* PlayerController = Cast<AController>(PlayerPawn->GetController());
	if (PlayerController == nullptr) return;

	FVector Location;
	FRotator Rotation;
	PlayerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * 5000.0f;

	FHitResult HitRes;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(this);
	FVector ParticlePosition = BulletSpawnPosition->GetComponentLocation();

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitRes, Location, End, ECollisionChannel::ECC_Camera, QueryParams);
	WeaponData.WeaponAmmo--;

	if (bHit) {
		AActor* HittedActor = HitRes.GetActor();

		if (HittedActor) {
			if (FireParticle) {
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireParticle, BulletSpawnPosition->GetComponentLocation(), BulletSpawnPosition->GetComponentRotation());
			}
			if (AEnemy* Enemy = Cast<AEnemy>(HittedActor)) {
				float Damage = FMath::RandRange(WeaponData.WeaponDamageMin, WeaponData.WeaponDamageMax);
				Enemy->TakeDamageEnemy(Damage);
				if (HitParticle) {
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticle, HitRes.Location, BulletSpawnPosition->GetComponentRotation());
				}
			}
			else {
				if (HitParticle) {
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticle, HitRes.Location, BulletSpawnPosition->GetComponentRotation());
				}
			}
		}
	}
	else {
		if (FireParticle) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireParticle, BulletSpawnPosition->GetComponentLocation(), BulletSpawnPosition->GetComponentRotation());
		}
	}
}

void AWeapon::Reload()
{
	if (WeaponData.WeaponAmmo != WeaponData.WeaponMaxAmmo) {
		WeaponData.WeaponAmmo = WeaponData.WeaponMaxAmmo;

		AHCharacter* Player = Cast<AHCharacter>(GetOwner());
		if (Player == nullptr) return;

		Player->GetPlayerHUD()->UpdateWeaponHUD(WeaponData);
	}
}

void AWeapon::PullTrigger()
{
	if (WeaponData.WeaponAmmo > 0) {
		AHCharacter* Player = Cast<AHCharacter>(GetOwner());
		if (Player == nullptr) return;

		Shoot();

		Player->GetPlayerHUD()->UpdateWeaponHUD(WeaponData);
	}
	else {
		//Reload();
	}
}

const FWeaponData AWeapon::GetWeaponData() const
{
	return WeaponData;
}


