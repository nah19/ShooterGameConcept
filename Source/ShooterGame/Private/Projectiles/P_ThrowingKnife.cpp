// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "D_AssaultWeapon.h"
#include "P_ThrowingKnife.h"
#include "ConceptCharacter.h"

AP_ThrowingKnife::AP_ThrowingKnife(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) 
{
	PrimaryActorTick.bCanEverTick = true;
	Contador = 0;

}
void AP_ThrowingKnife::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator::MakeFromEuler(FVector::ZeroVector));
	
}

void AP_ThrowingKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AConceptCharacter* Character = Cast<AConceptCharacter>(GetOwner());
	if (!Character)
	{
		return;
	}
	if (Character->GetLookingTo() == ELookingToEnum::RE_LOOKINGTOLEFT)
	{
		RotateLeft(DeltaTime);
	}
	else if (Character->GetLookingTo() == ELookingToEnum::RE_LOOKINGTORIGHT)
	{
		RotateRight(DeltaTime);
	}
	
}

void AP_ThrowingKnife::RotateLeft(float DeltaTime)
{
	Rotate(RotationSpeed * DeltaTime);
}

void AP_ThrowingKnife::RotateRight(float DeltaTime)
{
	Rotate(-RotationSpeed * DeltaTime);
}

void AP_ThrowingKnife::Rotate(float Direction)
{
	FVector Rotation = GetActorRotation().Euler();

	float NewRotation = Rotation.X + Direction;
	
	if (Direction > 0.0f && NewRotation > 360.0f)
		NewRotation -= 360.0f;
	else if (Direction < 0.0f && NewRotation < 0.0f)
		NewRotation += 360.0f;
	
	Rotation.X = NewRotation;
	
	FRotator Rotator = FRotator::MakeFromEuler(Rotation);
	
	SetActorRotation(Rotator);
	
}

void AP_ThrowingKnife::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit)
{
	
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character && Contador == 0) {
		UGameplayStatics::ApplyDamage(ActorToAffectParam, DamageBase, (AController*)Character->Controller, this, DamageType);
		Contador++;
	}

	//Destroy();
}

