// All rights reserved Life Before Studios ©

//TO DO: WYCZYŚCIĆ KOD Usunąć hanging tylko pierdolnąc na climbup i elo

#include "Character/Components/ClimbingComponent.h"
#include "Character/HCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

UClimbingComponent::UClimbingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bCanClimbUpAfterJump = false;
	bCanClimbUp = false;
}

void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HeightTrace();
}

void UClimbingComponent::HeightTrace()
{
	if(AHCharacter * Player = Cast<AHCharacter>(GetOwner())) {
		FHitResult HitRes;
		FVector Start = Player->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f) + Player->GetActorForwardVector() * 75.0f;
		FVector End = Start - FVector(0.0f, 0.0f, 100.0f);
		const FCollisionShape CollShape = FCollisionShape::MakeSphere(5.0f);
		const bool Hit = GetWorld()->SweepSingleByChannel(HitRes, Start, End, FQuat::Identity, ECC_GameTraceChannel2, CollShape);

		if (Hit) {
			FVector SocketLocation = Player->GetMesh()->GetSocketLocation("ClimbSocketHips");
			FVector HitLocation = HitRes.Location;
			float ZLocation = SocketLocation.Z - HitLocation.Z;
			if ((ZLocation > -50.0f && ZLocation < 0.0f)) {
				bCanClimbUp = true;
			}
			else if ((ZLocation > -100.0f && ZLocation < 0.0f)) {
				bCanClimbUp = false;
				bCanClimbUpAfterJump = true;
			}
			else {
				bCanClimbUp = false;
			}

			if (bCanClimbUpAfterJump && Player->GetCharacterMovement()->IsFalling() && (ZLocation > -50.0f && ZLocation < 0.0f)) {
				bCanClimbUpAfterJump = false;
				bCanClimbUp = true;
				ClimbUp();
			}
		}
		else {
			bCanClimbUp = false;
		}
	}
}

void UClimbingComponent::ClimbUp()
{
	if (bCanClimbUp) {
		if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
			if (M_HungToClimb) {
				Player->PlayHungAnimationBlueprint();
			}
		}
	}
}

void UClimbingComponent::OnClimbAnimationNotify()
{
	if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {

		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		FVector ForwardVec = Player->GetActorForwardVector() * 50.0f;
		FVector Vec = Player->GetRootComponent()->GetRelativeLocation() + ForwardVec;
		FVector TargetLocation = FVector(Vec.X, Vec.Y, Vec.Z + 155.0f);
		UKismetSystemLibrary::MoveComponentTo(Player->GetRootComponent(), TargetLocation, UKismetMathLibrary::MakeRotFromX(Player->GetActorForwardVector()), true, true, 1.0f, true, EMoveComponentAction::Move, LatentInfo);

		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		bCanClimbUp = false;
		bCanClimbUpAfterJump = false;
	}
}
