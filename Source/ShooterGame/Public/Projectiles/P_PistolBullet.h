// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/P_Bullet.h"
#include "P_PistolBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_PistolBullet : public AP_Bullet
{
	GENERATED_BODY()

public:

	AP_PistolBullet(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float BulletEffectForce = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float MaxAngle = 90;

	UFUNCTION(Server, WithValidation, Reliable)
		void SetPosition(FVector_NetQuantize10 Position);

	UFUNCTION(NetMulticast, Reliable)
		void SetPosition_Clients(FVector_NetQuantize10 Position);

private:
	void CalculateMovement(AConceptPlayerController* Controller);

protected:
	FVector Position;
	FVector BulletDirection;
	FVector MousePosition;
	FVector Bullet2Mouse;
	FVector NewDirection;

	bool bKeepFollowing = true;
};
