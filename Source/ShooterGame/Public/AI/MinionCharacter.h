// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Projectile.h"
#include "ConceptCharacter.h"
#include "MinionController.h"
#include "ConceptDamageBehavior.h"
#include "MinionCharacter.generated.h"

 UENUM(BlueprintType)
 enum class ELanePosition : uint8 {
	RE_ALL	UMETA(DisplayName = "Any lane"),
	RE_TOP	UMETA(DisplayName = "Top lane"),
	RE_MID	UMETA(DisplayName = "Mid lane"),
	RE_BOT	UMETA(DisplayName = "Bot lane"),
	RE_NOT	UMETA(DisplayName = "No lane"),
	
};

UCLASS()
class SHOOTERGAME_API AMinionCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	FVector Gravity;
	FRotator LeftOrientation;
	FRotator RightOrientation;
	FRotator FloorOrientation;
	float RotationSpeed = 1.8f;
    bool bIsDead = false;

	
	AMinionCharacter(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Minion")
		UParticleSystem* MuzzleClass;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Minion")
		UParticleSystem* ExplosionClass;
//	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Minion")
//		USphereComponent* CollisionAvoiderComponent;


	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "Minion")
		float Health;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "Minion")
		float MaxHealt;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "Minion")
		float Armor;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "Minion")
		float Speed;
	UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "Minion")
		float JumpPower;
	UPROPERTY(replicated, editanywhere, blueprintreadwrite, Category = "Minion")
		int32 TeamNumber;
	UPROPERTY(replicated, editanywhere, blueprintreadwrite, Category = "Minion")
		AActor* Target_C;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		TArray<FRotator> MapPawnOrientations;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, category = "ConceptCharacter")
		FRotator CurrentFloorOrientation;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		bool isLerpingGravity = true;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "Minion")
		FGenericMinionData MinionData;
	UPROPERTY()
		UConceptDamageBehavior* MyDamageBehavior;
	UPROPERTY()
		bool CanReceiveDamage = false;
	UPROPERTY(replicated, visibleanywhere, blueprintreadwrite, Category = "Minion")
		bool LockMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minion")
		float DifficultPoints;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Minion")
		void DissolveMaterialEffect();
	UFUNCTION(NetMulticast, Reliable)
		void Death();
	UFUNCTION(Server, Reliable, Withvalidation, BlueprintCallable, Category = "Minion")
		void Attack(FVector StartLocation, FRotator StartRotation, TSubclassOf<class AProjectile> ProjectileClass);
	UFUNCTION(NetMulticast, Reliable)
		void ShotReplicated(FVector StartLocation, FRotator StartRotation, TSubclassOf<class AProjectile> ProjectileClassParam);
	UFUNCTION()
		AProjectile* SpawnProjectile(FVector StartLocation, FRotator StartRotation, TSubclassOf<class AProjectile> ProjectileClass);
	UFUNCTION(Category = "Pawn|CustomCharacter", BlueprintCallable)
		virtual void LaunchCharacterRotated(FVector LaunchVelocity, bool bHorizontalOverride, bool bVerticalOverride);
	UFUNCTION(BlueprintCallable, Category = "Game")
		void OnMinionDestroyed();
//	UFUNCTION(BlueprintCallable, Category = "Minion")
//		void OnCollisionAvoidOverlap(UPrimitiveComponent* MyComp, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION(BlueprintCallable, Category = "Minion")
		bool IsPointAtRightOfLine(FVector PointA, FVector PointB, FVector PointToCheck);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Jump() override;
	void NavigationJump(FNavPathPoint StartPoint, FNavPathPoint EndPoint);
	UFUNCTION(BlueprintCallable, Category = "MinionNavigation")
	void JumpFromTo(FVector StartPoint, FVector EndPoint);
	UFUNCTION(BlueprintCallable, Category = "MinionNavigation")
	void JumpFromToWithFixedHeight(FVector StartPoint, FVector EndPoint, float JumpTime = 1.0f);
	virtual void ApplyDamageMomentum(float DamageTaken, const FDamageEvent& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;
	virtual FVector GetPawnViewLocation() const override;
	virtual void PostNetReceiveLocationAndRotation() override;
	FORCEINLINE class UCustomCharacterMovementComponent* GetCustomCharacterMovement() const;
	void GravityRotatorCalculator(float DeltaSeconds);
	

	FRotator RotationLerp(FRotator InitRotation, FRotator FinalRotation, float alpha);

protected:
	bool isNavJumping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion")
		TSubclassOf<class AActor> AltenumClass;
	UPROPERTY()
		AActor* Altenum;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
		void SpawnParticle(FVector PointToSpawn, FRotator Rotation, FVector Size, UParticleSystem* ParticleClass, FName Socket);
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	UFUNCTION()
		void EnablePhysics();
	UFUNCTION()
		void OnExitHover();
	UFUNCTION()
		void OnHover();
	UFUNCTION()
		void FillMinionVariablesWithData(UPARAM(ref) FGenericMinionData& DataParam);
	UFUNCTION(BlueprintCallable, Category = "Game")
		float SetCharacterSpeed(float SpeedParam);
	UFUNCTION(BlueprintCallable, Category = "Game")
		float SetCharacterJump(float JumpPowerParam);
	UFUNCTION()
		void SpawnAltenumCollectable();

  	

	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);

private:
	float currentDeltaTime;
	float StuckTime;
};
