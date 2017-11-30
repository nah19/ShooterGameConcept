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

// Calcula la trayector�a que ha de seguir la bala
void AP_PistolBullet::CalculateMovement(AConceptPlayerController* Controller)
{
	// Obten el vector direcci�n del movimiento de la bala y el vector direcci�n de 
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

	// Comprueba si ha de seguir la bala al cursor y calcula el �ngulo entre los dos vectores
	if (bKeepFollowing && UKismetMathLibrary::RadiansToDegrees(acosf(FVector::DotProduct(BulletDirection, Bullet2Mouse))) < MaxAngle)
	{
		// El nuevo vector direcci�n se calcula interpolando Direcci�nActual-Direcci�nAlCursor
		NewDirection = FMath::Lerp(BulletDirection, Bullet2Mouse, BulletEffectForce);
		
		SetPosition(NewDirection);
	}
	// Si el �ngulo sobrepasa el m�ximo �ngulo, deja de seguir al cursor
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