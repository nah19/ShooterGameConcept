// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectiles/P_Grenade.h"
#include "P_ThrowingKnife.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_ThrowingKnife : public AP_Grenade
{
	GENERATED_BODY()

public:
	AP_ThrowingKnife(const FObjectInitializer& ObjectInitializer);

	virtual void ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) override;

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwing Knife")
		float RotationSpeed = 0.0f;

private:

	void RotateLeft(float DeltaTime);

	void RotateRight(float DeltaTime);

	void Rotate(float Direction);

	int32 Contador;


};
