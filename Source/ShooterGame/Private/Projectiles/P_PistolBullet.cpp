// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "P_PistolBullet.h"
#include "ConceptCharacter.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

AP_PistolBullet::AP_PistolBullet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	NetUpdateFrequency = 15;
}

void AP_PistolBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AConceptCharacter* Character = Cast<AConceptCharacter>(GetOwner());
	if (Character) {
		AConceptPlayerController* MyController = Cast<AConceptPlayerController>(GetOwner()->GetInstigatorController());
		if(MyController && MyController->IsLocalController())
		{
			CalculateMovement(MyController);
		}	
	}
}

// Calcula la trayectoría que ha de seguir la bala
void AP_PistolBullet::CalculateMovement(AConceptPlayerController* Controller)
{
	// Obten el vector dirección del movimiento de la bala y el vector dirección de 
	BulletDirection = GetActorForwardVector();
	if (!Controller->CalculateCursorPosition().ContainsNaN())
	{
		Bullet2Mouse = Controller->CalculateCursorPosition() - GetActorLocation();

		// Normaliza los vectores
		BulletDirection.Normalize();
		Bullet2Mouse.Normalize();
	}
	else
	{
		bKeepFollowing = false;
	}

	// Comprueba si ha de seguir la bala al cursor y calcula el ángulo entre los dos vectores
	if (bKeepFollowing && UKismetMathLibrary::RadiansToDegrees(acosf(FVector::DotProduct(BulletDirection, Bullet2Mouse))) < MaxAngle)
	{
		// El nuevo vector dirección se calcula interpolando DirecciónActual-DirecciónAlCursor
		NewDirection = FMath::Lerp(BulletDirection, Bullet2Mouse, BulletEffectForce);
		
		SetPosition(NewDirection);
	}
	// Si el ángulo sobrepasa el máximo ángulo, deja de seguir al cursor
	else if (bKeepFollowing)
	{
		bKeepFollowing = false;
	}
}

void AP_PistolBullet::SetPosition_Implementation(FVector_NetQuantize10 Position)
{
	SetActorRotation(Position.Rotation());

	ProjectileMovementComponent->SetVelocityInLocalSpace(Position.ForwardVector * ProjectileMovementComponent->InitialSpeed);

	SetPosition_Clients(Position);
}

bool AP_PistolBullet::SetPosition_Validate(FVector_NetQuantize10 Position)
{
	return true;
}

void AP_PistolBullet::SetPosition_Clients_Implementation(FVector_NetQuantize10 Position)
{
	SetActorRotation(Position.Rotation());

	ProjectileMovementComponent->SetVelocityInLocalSpace(Position.ForwardVector * ProjectileMovementComponent->InitialSpeed);
}