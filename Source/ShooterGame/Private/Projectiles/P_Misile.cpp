// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_Misile.h"
#include "Ability.h"
#include "D_AssaultWeapon.h"
#include "StaticFunctionLibrary.h"
#include "ConceptCharacter.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

AP_Misile::AP_Misile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	NetUpdateFrequency = 15;
}

void AP_Misile::BeginPlay(){
	Super::BeginPlay();
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	if (MyCharacter) {
		Target = MyCharacter->SelectedTarget;
	}
	
}

void AP_Misile::Tick(float DeltSeconds) {
	Super::Tick(DeltSeconds);

	if (!bIsRemoteControlled) return;

	AConceptCharacter* Character = Cast<AConceptCharacter>(GetOwner());
	if (Character) {
		AConceptPlayerController* MyController = Cast<AConceptPlayerController>(GetOwner()->GetInstigatorController());

		if (MyController && MyController->IsLocalController())
		{
			CalculateMovement(MyController);
		}
	}
}

void AP_Misile::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {

	EffectVisual(ActorToAffectParam, Hit);

	EffectLogic(ActorToAffectParam, Hit);

	Destroy();
}

void AP_Misile::EffectVisual(AActor* ActorToAffectParam, const FHitResult& Hit){
	AConceptCharacter* CharacterHitted = Cast<AConceptCharacter>(Hit.GetActor());
	if (!CharacterHitted && ImpactDecal) {
		UDecalComponent* LocalDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactDecal->GetMaterial(), FVector(40, SizeOfDecal, SizeOfDecal), Hit.Location, (Hit.ImpactNormal * -1).Rotation(), 10);
		LocalDecal->SetWorldLocation(Hit.Location);
		LocalDecal->SetWorldRotation((Hit.ImpactNormal * -1).Rotation());
		LocalDecal->SetFadeOut(6, 10);
	}
}


void AP_Misile::EffectLogic(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character) {
		UGameplayStatics::ApplyRadialDamage(this, DamageBase, GetActorLocation(), EffectArea, DamageType, TArray<AActor*>(), this, (AController*)Character->Controller, true, ECollisionChannel::ECC_GameTraceChannel5);
	}
}

void AP_Misile::Explode() {
	Destroy();
}


void AP_Misile::CalculateMovement(AConceptPlayerController* Controller)
{
	BulletDirection = GetActorForwardVector();
	if (!Controller->CalculateCursorPosition().ContainsNaN())
	{
		Bullet2Mouse = Controller->CalculateCursorPosition() - GetActorLocation();
		BulletDirection.Normalize();
		Bullet2Mouse.Normalize();

		NewDirection = FMath::Lerp(BulletDirection, Bullet2Mouse, BulletEffectForce * GetWorld()->DeltaTimeSeconds);

		SetPosition(NewDirection);
	}
}

void AP_Misile::SetPosition_Implementation(FVector_NetQuantize10 Position)
{
	SetPosition_Clients(Position);
}

bool AP_Misile::SetPosition_Validate(FVector_NetQuantize10 Position)
{
	return true;
}

void AP_Misile::SetPosition_Clients_Implementation(FVector_NetQuantize10 Position)
{
	SetActorRotation(Position.Rotation());

	ProjectileMovementComponent->SetVelocityInLocalSpace(Position.ForwardVector * ProjectileMovementComponent->InitialSpeed);
}

float AP_Misile::normalizeAngle(float angle) {
	if (angle < 0)
		angle += 360;
	else if (angle >= 360)
		angle -= 360;
	return angle;
}