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

	bIsHunging = false;
	bCheckForFloorContact = false;
	bCanHung = false;
	bCanHungAfterJump = false;
	bCanClimbUp = false;
}

void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceForward();
	HeightTrace();

	if (bCheckForFloorContact) {
		CheckIfIsGrounded();
	}
}

void UClimbingComponent::TraceForward()
{
	if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
		FHitResult HitRes;
		FVector Start = Player->GetActorLocation();
		FVector End = Start + Player->GetActorForwardVector() * 150.0f;
		const FCollisionShape CollShape = FCollisionShape::MakeSphere(5.0f);
		const bool Hit = GetWorld()->SweepSingleByChannel(HitRes, Start, End, FQuat::Identity, ECC_GameTraceChannel2, CollShape);
		WallNormal = HitRes.Normal;
		WallLocation = HitRes.Location;
		if (Hit) {
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1.0f, 0U, 5.0f);
			DrawDebugSphere(GetWorld(), HitRes.Location, 10.0f, 10, FColor::White);
		}
		else {
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.0f, 0U, 5.0f);
		}
		
	}
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
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1.0f, 0U, 5.0f);
			DrawDebugSphere(GetWorld(), HitRes.Location, 10.0f, 10, FColor::White);
		}
		else {
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.0f, 0U, 5.0f);
		}

		if (Hit) {
			FVector SocketLocation = Player->GetMesh()->GetSocketLocation("ClimbSocketHips");
			FVector HitLocation = HitRes.Location;
			HeightLocation = HitLocation;
			float ZLocation = SocketLocation.Z - HitLocation.Z;
			if (ZLocation > -50.0f && ZLocation < 0.0f) {
				bCanClimbUp = true;
				bCanHung = false;
			}
			else if ((ZLocation > -100.0f && ZLocation < 0.0f)) {
				bCanHungAfterJump = true;
				if (!bIsHunging) {
					bCanClimbUp = false;
				}
			}
			else {
				if(!bIsHunging){
					bCanHung = false;
					bCanClimbUp = false;
				}
			}
			if (bCanHungAfterJump && Player->GetCharacterMovement()->IsFalling() && (ZLocation > -50.0f && ZLocation < 0.0f)) {
				bCanHungAfterJump = false;
				bCanHung = true;
				//Hung();
				ClimbUp();
				bCanClimbUp = true;
			}
		}
		else {
			if (!bIsHunging) {
				bCanHung = false;
				bCanClimbUp = false;
			}
		}
	}
}

void UClimbingComponent::CancelHunging()
{
	if (bIsHunging) {
		if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
			Player->PlayAnimMontage(M_HungToDrop, 1.0f);
			Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}
		bCheckForFloorContact = true;
	}
}

void UClimbingComponent::CheckIfIsGrounded()
{
	if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
		if (!Player->GetCharacterMovement()->IsFalling()) {
			bCheckForFloorContact = false;
			bIsHunging = false;
		}
	}
}

void UClimbingComponent::SetupClimbingInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(HungToClimbUpAction, ETriggerEvent::Started, this, &UClimbingComponent::ClimbUp);
	EnhancedInputComponent->BindAction(HungToCancel, ETriggerEvent::Started, this, &UClimbingComponent::CancelHunging);
}


void UClimbingComponent::ClimbUp()
{
	if (bCanClimbUp) {
		if (bIsHunging) {
			bIsHunging = false;
			ClimbUp();
		}
		else { //
			if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
				if (M_HungToClimb) {
					Player->PlayAnimationBlueprint();
				}
			}
		}
	}
}

void UClimbingComponent::Hung()
{
	//if (bCanHung) {
		bCanHung = false;
		if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
			FVector NewWallNormal = WallNormal * 30.0f + WallLocation;
			FVector TargetRelativeLocation = FVector(NewWallNormal.X, NewWallNormal.Y, HeightLocation.Z - 120.0f);

			if (M_Hung) {
				FLatentActionInfo LatentInfo;
				LatentInfo.CallbackTarget = this;
				Player->PlayAnimMontage(M_HungToClimb, 1.0f);
				FRotator Rotation = UKismetMathLibrary::MakeRotFromX(WallNormal * -1.0f);
				UKismetSystemLibrary::MoveComponentTo(Player->GetRootComponent(), TargetRelativeLocation, UKismetMathLibrary::MakeRotFromX(WallNormal * -1.0f), true, true, 0.5f, true, EMoveComponentAction::Move, LatentInfo);
				Player->GetController()->SetControlRotation(FRotator(Rotation.Pitch + 50.0f, Rotation.Yaw, Rotation.Roll));
			}
			Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			Player->GetCharacterMovement()->StopMovementImmediately();
			bIsHunging = true;
			bCanClimbUp = true;
		}
	//}
}

const bool UClimbingComponent::bGetCanClimbUp()
{
	return bCanClimbUp;
}

const bool UClimbingComponent::bGetCanHungAfterJump()
{
	return bCanHungAfterJump;
}

const bool UClimbingComponent::bGetCanHung()
{
	return bCanHung;
}

const bool UClimbingComponent::bGetIsHunging()
{
	return bIsHunging;
}

void UClimbingComponent::OnClimbAnimationNotify()
{
	if (AHCharacter* Player = Cast<AHCharacter>(GetOwner())) {
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		FVector ForwardVec = Player->GetActorForwardVector() * 50.0f;
		FVector Vec = Player->GetRootComponent()->GetRelativeLocation() + ForwardVec;
		FVector TargetLocation = FVector(Vec.X, Vec.Y, Vec.Z + 192.0f);
		UKismetSystemLibrary::MoveComponentTo(Player->GetRootComponent(), TargetLocation, UKismetMathLibrary::MakeRotFromX(Player->GetActorForwardVector()), true, true, 1.0f, true, EMoveComponentAction::Move, LatentInfo);
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		bCanClimbUp = false;
		bCanHung = false;
		bCanHungAfterJump = false;
	}
}

