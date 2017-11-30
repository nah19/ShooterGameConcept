// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "ConceptPlayerController.h"
#include "ConceptPlayerState.h"
#include "ConceptDamageBehavior.h"
#include "StaticFunctionLibrary.h"
#include "ConceptCharacter.generated.h"


class AAbility;

UENUM(BlueprintType)
namespace ELookingToEnum {
	enum LookingTo {
		RE_LOOKINGTORIGHT 	UMETA(DisplayName = "LOOKINGTORIGHT"),
		RE_LOOKINGTOLEFT 	UMETA(DisplayName = "LOOKINGTOLEFT"),
	};
}

UENUM(BlueprintType)
enum class ELookingToEnumCustom : uint8 {
	RE_LOOKINGTORIGHT 	UMETA(DisplayName = "LOOKINGTORIGHT"),
	RE_LOOKINGTOLEFT 	UMETA(DisplayName = "LOOKINGTOLEFT")
};

UENUM(BlueprintType)
namespace EOrientationToEnum {
	enum OrientedTo {
		RE_BACKWARD 	UMETA(DisplayName = "RE_BACKWARD"),
		RE_FORWARD 	UMETA(DisplayName = "RE_FORWARD"),
		RE_ANY 	UMETA(DisplayName = "RE_ANY"),
	};
}

UENUM(BlueprintType)
namespace ECharacterStateEnum {
	enum CharacterState {
		RE_NORMAL		UMETA(DisplayName = "RE_NORMAL"),
		RE_ZEROGRAVITY	UMETA(DisplayName = "RE_ZEROGRAVITY")
	};
}

UENUM(BlueprintType)
namespace ECharacterCCStatusEnum {
	enum CharacterCCStatus {
		RE_SLOWED		UMETA(DisplayName = "RE_SLOWED"),
		RE_STUNNED		UMETA(DisplayName = "RE_STUNNED"),
		RE_SNARED		UMETA(DisplayName = "RE_SNARED"),
		RE_BURNED		UMETA(DisplayName = "RE_BURNED"),
		RE_POISONED		UMETA(DisplayName = "RE_POISONED"),
		RE_DISRUPTED	UMETA(DisplayName = "RE_DISRUPTED")
	};
}



UCLASS(config = Game, Blueprintable)
class SHOOTERGAME_API AConceptCharacter : public ACharacter {
	GENERATED_BODY()

public:
	AConceptCharacter(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE class UCustomCharacterMovementComponent* GetCustomCharacterMovement() const;
	UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "CharacterStatus")
		UParticleSystemComponent* DisruptParticleStatusComp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Sound)
		UAudioComponent* BreathSoundComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Force)
		URadialForceComponent* RadialForceComponent;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Camera)
		UCameraComponent* SideViewCameraComponent;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
	bool bAbleToShoot;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
	bool bAbleToDash;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
	float CamaraRotationMultiplier = 1;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
	bool bUseGravity;
	UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "CharacterStatus")
		UParticleSystem* DisruptParticleStatus;


	
	
	template<int32 Index>
	void AbilityTestInUse();

	float LerpSpeed = 0.3f;
	float AimRefreshRate = 0.3f;


	FTimerHandle ChangeAbilityHandle;
	FTimerDelegate ChangeAbilityDelegate;

	FTimerHandle AimingToHandler;
	FTimerDelegate AimingToDelegate;

	FTimerHandle UpdatesBasedInMouseStatsHandle;
	FTimerDelegate UpdatesBasedInMouseStatsDelegate;

	FTimerHandle UpdateCameraWhenIsFreeHandle;
	FTimerDelegate UpdateCameraWhenIsFreeDelegate;

	FTimerHandle StaminaExpenseSystemHandle;
	FTimerDelegate StaminaExpenseSystemDelegate;

	FTimerHandle StaminaRegenerationSystemHandle;
	FTimerDelegate StaminaRegenerationSystemDelegate;

	FTimerHandle LifeRegenerationHandle;
	FTimerDelegate LifeRegenerationDelegate;

	

	FTimerHandle ShieldHandle;
	FTimerDelegate ShieldDelegate;

	FTimerHandle ShieldRegenerationHandle;
	FTimerDelegate ShieldRegenerationDelegate;

	FTimerHandle DisruptHandle;
	FTimerDelegate DisruptDelegate;

	FTimerHandle ConstrictionHandle;
	FTimerDelegate ConstrictionDelegate;

	FTimerHandle SlowStatusHandle;
	FTimerDelegate SlowStatusDelegate;

	FTimerHandle StunHandle;
	FTimerDelegate StunDelegate;

    FTimerHandle SlowHandle;
    FTimerDelegate SlowDelegate;
    
	FTimerHandle SnareHandle;
	FTimerDelegate SnareDelegate;

	UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "ConceptCharacter")
	FString NameOfCharacter;

	UPROPERTY(Replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		AConceptPlayerController* MyPlayerController;
	UPROPERTY(Replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		AConceptPlayerState* MyPlayerState;
	UPROPERTY(Replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		AConceptGameMode* MyGameMode;
	UPROPERTY(Replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		AConceptGameState* MyGameState;

	//SETTING UP PROPERTIES
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "ConceptCharacter")
		UTexture2D* MinimapImage;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "ConceptCharacter")
		UTexture2D* CharacterImage;
	UPROPERTY(editanywhere, blueprintreadwrite, Category = "ConceptCharacter")
		FGenericCharacterData Data;

	//SKILLS PROPERTIES
	//UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
	//	FVector_NetQuantize Location;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float Life;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float LifeMax;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float LifeReg;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float Shield;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float ShieldMax;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float ShieldReg;

	//Replicates???
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float Armor;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float Speed;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float SprintSpeed;
	UPROPERTY(VisibleInstanceOnly, replicated, blueprintreadonly, Category = "ConceptCharacter")
		float VelocityMultiplierOnUse;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float JumpPower;
	UPROPERTY(blueprintreadwrite, EditInstanceOnly, Category = "ConceptCharacter")
		float Stamina;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float StaminaMax;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float StaminaReg;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float StaminaExp;



	//STATE PROPERTIES
	UPROPERTY()
		bool IsRegenerating;
	UPROPERTY()
		bool IsChangingAbility;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		bool IsSprinting;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		bool IsVisible = true;
	UPROPERTY()
		bool bInsideTurret;
	UPROPERTY(replicated, EditInstanceOnly, BlueprintReadWrite, Category = "ConceptCharacter")
		int32 IncrementOfWheel;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		TArray<AConceptPlayerController*> Damagers;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		TArray<float> TimesSinceLastDamage;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		AAbility* AbilityOfMontage;
	UPROPERTY(replicated, EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		bool bIsFiringUnique;
	UPROPERTY(replicated, blueprintreadwrite, EditInstanceOnly, Category = "ConceptCharacter")
		AAbility* FiringUnique;
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Character")
		TEnumAsByte<ECharacterStateEnum::CharacterState> CharacterState;
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Character")
		TArray<TEnumAsByte<ECharacterCCStatusEnum::CharacterCCStatus>> CharacterCCStatus;

	//OTHER PROPERTIES
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		TArray<FRotator> MapPawnOrientations;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, category = "ConceptCharacter")
		FRotator CurrentFloorOrientation;
	UPROPERTY(blueprintreadonly, VisibleInstanceOnly, category = "ConceptCharacter")
		bool bOtherHasReceivedDamage;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Material)
		TArray<UMaterialInterface*> DefaultsMaterial;
	UPROPERTY()
		bool ShouldTurn;
	UPROPERTY(replicated, blueprintreadonly, VisibleInstanceOnly, category = "ConceptCharacter")
		FVector2D LinearVectorToAim;
	UPROPERTY(blueprintreadwrite, VisibleInstanceOnly,  category = "ConceptCharacter")
		FVector2D LocalLinearVectorToAim;
    UPROPERTY(blueprintreadwrite, VisibleInstanceOnly,  category = "ConceptCharacter")
    bool bAutoAim;
	UPROPERTY(Replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		int32 FiringType;
	UPROPERTY()
		TSubclassOf<UCameraShake> RunningCameraShake;
	UPROPERTY()
		float TimeSinceLastMovement;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		AAbility* AbilityInUse;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		bool SprintKeyPressed;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, replicated)
		AActor* SelectedTarget;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		bool IsRunningForward;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "ConceptCharacter")
		bool IsPlayingMontage;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		bool isOnAir;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float MoveRightValue;

	UPROPERTY()
		bool CanReceiveDamage;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		float TimeToReceiveDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Status")
		bool bPoisoned;
	UPROPERTY(BlueprintReadOnly, Category = "Status")
		bool bBurning;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
		bool bConstrictioning;

	UPROPERTY(replicated)
		bool InversePlayer = false;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		bool FreeCamera;
	UPROPERTY()
		UConceptDamageBehavior* MyDamageBehavior;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		bool bIsShieldActivated;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
        float AxisX;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
        float AxisY;
    UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
        bool bGameAxisInactive;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float UpAxisMovement;
	UPROPERTY()
		float RightAxisMovement;
	UPROPERTY(EditInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		bool isLerpingGravity = true;
	UPROPERTY(VisibleInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		bool bWantsFire = false;
	UPROPERTY()
		bool bStaminaRegeneration = true;
	UPROPERTY(VisibleInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		FVector AimingPadVector;
	UPROPERTY(VisibleInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		FVector MovementPadVector;
	UPROPERTY(VisibleInstanceOnly, blueprintreadwrite, Category = "ConceptCharacter")
		FVector MovementVector;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		FVector VelAux;
	UPROPERTY(EditAnywhere, blueprintreadonly, Category = "ConceptCharacter")
		USoundCue* BreathCue;
	UPROPERTY(EditAnywhere, blueprintreadonly, Category = "ConceptCharacter")
		USoundCue* HurtCue;
	UPROPERTY(EditAnywhere, blueprintreadonly, Category = "ConceptCharacter")
		USoundCue* DeathCue;
	UPROPERTY()
		USoundCue* TriggerCue;
	UPROPERTY(EditAnywhere, blueprintreadonly, Category = "ConceptCharacter")
		USoundCue* ArmorCue;




	UFUNCTION(Server, Reliable, WithValidation)
		void EnableShieldServer();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void ToggleShield();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void EnableShield();
	UFUNCTION(Server, Reliable, WithValidation)
		void DisableShieldServer();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void DisableShield();
	UFUNCTION()
		void AimAxisX(float MovementX);
	UFUNCTION()
		void AimAxisY(float MovementY);
	UFUNCTION()
		void GamePadAiming(float MovementX, float MovementY);
	UFUNCTION()
		void OnSprintPressed();
	UFUNCTION()
		void OnSprintReleased();
	UFUNCTION()
		void MoveRight(float Val);
	UFUNCTION()
		void MoveUp(float Val);
	UFUNCTION(BlueprintCallable, Category = "ConceptCharacter")
		void PlaySound(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "ConceptCharacter")
		void StopSound(UAudioComponent* Component);
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "ConceptCharacter")
		void PlaySound_Server(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol = 1.0f);
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "ConceptCharacter")
		void StopSound_Server(UAudioComponent* Component);
	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "ConceptCharacter")
		void PlaySound_Client(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol = 1.0f);
	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "ConceptCharacter")
		void StopSound_Client(UAudioComponent* Component);
	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = "ConceptCharacter")
		void PlaySound_Multicast(USoundCue* Sound, UAudioComponent* Component, bool IsAttached, float Vol = 1.0f);
	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = "ConceptCharacter")
		void StopSound_Multicast(UAudioComponent* Component);
	UFUNCTION()
		void MoveCameraWithFreedomPressed();
	UFUNCTION()
		void MoveCameraWithFreedomReleased();
	UFUNCTION()
		void SubstractDamage();
	UFUNCTION()
		void OnCrouchPressed();
	UFUNCTION()
		void OnCrouchReleased();
	UFUNCTION()
		void SetAbleToDash(bool Able);
	UFUNCTION()
		void UpdatesBasedInMouseStats();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void DissolveMaterialEffect();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void GameFinished();
	UFUNCTION(blueprintcallable, Category = "ConceptCharacter")
		EOrientationToEnum::OrientedTo MovingForward();
	UFUNCTION()
		void StartMovingAimTo();
	UFUNCTION(BlueprintCallable, Category = "ConceptCharacter")
		void StartStaminaRegeneration();
	UFUNCTION(BlueprintPure, Category = "ConceptCharacter")
		ELookingToEnum::LookingTo GetLookingTo();
	UFUNCTION(BlueprintCallable, Category = "ConceptCharacter")
		void InitCharacterData();
	UFUNCTION(BlueprintCallable, Category = "ConceptCharacter")
		void LerpForLinearRotation();
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "ConceptCharacter")
		void OtherHasReceivedDamage();
	UFUNCTION(Server, WithValidation, Unreliable, BlueprintCallable, Category = "ConceptCharacter")
		void SetLinearVectorToAim(FVector2D RotationToAim);
	UFUNCTION(NetMulticast, Reliable)
		void MakeInvisible(UMaterialInterface* NewMat, bool Visible = true);
	UFUNCTION(Server, withvalidation, blueprintcallable, Reliable, Category = "ConceptCharacter")
		void Server_MakeInvisible(UMaterialInterface* NewMat, bool Visible = true);
	UFUNCTION(Server, WithValidation, Reliable, blueprintcallable, Category = "ConceptCharacter")
		void StartChangeAbility(int32 IndexOfWeaponToUse);
	UFUNCTION(Server, WithValidation, Reliable, blueprintcallable, Category = "ConceptCharacter")
		void ChangeAbility();
	UFUNCTION(Server, WithValidation, Reliable, blueprintcallable, Category = "ConceptCharacter")
		void EndChangeAbility();
	UFUNCTION(NetMulticast, WithValidation, Reliable, blueprintcallable, Category = "ConceptCharacter")
		void SetAbilityVisibility(bool bIsVisible, AAbility* Ability);
	UFUNCTION()
		void Reload();
	UFUNCTION()
		void ReduceShield(float Reduction);
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "ConceptCharacter")
		void SetShield(float ShieldParam);
	UFUNCTION(blueprintcallable, Category = "ConceptCharacter")
		float SetCharacterJump(float SpeedParam);
	UFUNCTION(blueprintcallable, Category = "ConceptCharacter")
		float SetCharacterSpeed(float SpeedParam);
	UFUNCTION(NetMulticast, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void MulticastSetSpeed(float SpeedParam);
	UFUNCTION(Server, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void ServerSetSpeed(float SpeedParam);
	UFUNCTION(NetMulticast, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void MulticastSetSprintSpeed();
	UFUNCTION(Server, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void ServerSetSprintSpeed();
	UFUNCTION(NetMulticast, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void MulticastSetNormalSpeed();
	UFUNCTION(Server, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void ServerSetNormalSpeed();
	UFUNCTION(Server, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void ServerSetLife(float SpeedParam);
	UFUNCTION(Server, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void ServerSetLifeMax(float SpeedParam);
	UFUNCTION(NetMulticast, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void MulticastSetLifeReg(float SpeedParam);
	UFUNCTION(Server, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void ServerSetLifeReg(float SpeedParam);
	UFUNCTION(NetMulticast, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void MulticastSetJump(float SpeedParam);
	UFUNCTION(Server, Reliable, WithValidation, blueprintcallable, Category = "ConceptCharacter")
		void ServerSetJump(float SpeedParam);
	UFUNCTION(Server, Reliable, WithValidation)
		void LifeRegenerationSystem();
	UFUNCTION(Server, Reliable, WithValidation)
		void ShieldRegenerationSystem();
	UFUNCTION(Client, Reliable)
		void FireMainAbility();
	UFUNCTION(Client, Reliable)
		void StopMainAbility();
	UFUNCTION(Client, Reliable)
		void FireSecondaryAbility();
	UFUNCTION(Client, Reliable)
		void PerformSpecialAbility();
	UFUNCTION(Client, Reliable)
		void StopSecondaryAbility();
	UFUNCTION(Server, Reliable, WithValidation)
		void SetTarget(AActor* Target);
	UFUNCTION(NetMulticast, blueprintcallable, Reliable, Category = "ConceptCharacter")
		void Death();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ConceptCharacter")
		void ReactiveCharacter();
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ConceptCharacter")
		void CustomTurnOff();
	UFUNCTION(Server, Reliable, withvalidation)
		void SetCanReceiveDamage(bool CanReceive);
	UFUNCTION()
		void CalculateWherePlayerIsLookingTo(float MouseX, float CharacterX);
	UFUNCTION()
		void FillCharacterVariablesWithData(UPARAM(ref) FGenericCharacterData& DataParam);
	UFUNCTION(Server, WithValidation, Reliable)
		void CanShoot(bool bCanShoot);
	UFUNCTION(NetMulticast, Reliable)
		void CanShoot_Clients(bool bCanShoot);
	UFUNCTION(NetMulticast, WithValidation, Reliable)
		void SetOrientationToMovement_Multicast(bool OrientedToMovement);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetOrientationToMovement_Server(bool OrientedToMovement);
	UFUNCTION()
		void MakeRadialForce(float Radius, float Force, float TimeToStop, bool bShouldStop = false);
	UFUNCTION()
		void ShakeManager(TSubclassOf<class UCameraShake> ShakeClass, bool bWantStop, float Scale = 1.0f);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void OnLifeDamageReceive(float Damage, TSubclassOf<UDamageType> DamageType);
	UFUNCTION()
		void InitGravity();
	UFUNCTION(BlueprintCallable, Category = "ZeroGravity")
		void ActivateZeroGravity();
	UFUNCTION(BlueprintCallable, Category = "ZeroGravity")
		void DeactivateZeroGravity();
	UFUNCTION()
		void TemporalZeroGravity(float Time);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void EnablePhysics(float Time = 0.0f);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void OnCharacterHitted(float Time = 0.0f);
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "ConceptCharacter")
		void ReplicateGravity(bool bActivateZeroGravity, float Direction);
	UFUNCTION(NetMulticast, Reliable)
		void ReplicateGravity_Clients(bool bActivateZeroGravity, float Direction);
	UFUNCTION(BlueprintCallable, Category = "StatusSystem")
		void EnableMovement();
	UFUNCTION(BlueprintCallable, Category = "StatusSystem")
		void DisableMovement();
	UFUNCTION(BlueprintCallable, Category = "StatusSystem")
		void ToggleAbilities(bool ActiveStatus);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void OnCCStatusUpdated();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "StatusSystem")
		void StartDisruptStatus(float DisruptTime);
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "StatusSystem")
		void StartDisruptStatusServer(float DisruptTime);
	//Slow
	//UFUNCTION(Client, Reliable, BlueprintCallable, Category = "StatusSystem")
	//	void StartSlowStatus(float SlowAmount, float SlowTime);
	//UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "StatusSystem")
	//	void StartSlowStatusServer(float SlowAmount, float SlowTime);
    UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "StatusSystem")
    void StartSlowStatusServer(float SlowTime, float SlowAmount);
    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "StatusSystem")
    void StartSlowStatus(float SlowTime, float SlowAmount);
    //Stun
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "StatusSystem")
		void StartStunStatusServer(float StunTime);
    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "StatusSystem")
        void StartStunStatus(float StunTime);
	//snare
	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "StatusSystem")
		void StartSnareStatusServer(float SnareTime);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "StatusSystem")
		void StartSnareStatus(float SnareTime);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyPlayerState(AConceptPlayerState* Char);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyPlayerController(AConceptPlayerController* Char);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyGameMode(AConceptGameMode* Char);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetMyGameState(AConceptGameState* Char);
	UFUNCTION()
		void OnGetAchievement();
	UFUNCTION()
		void SetPoisonStatus(AController* Enemy, float Damage, float poisonStatusTimeLeft, float poisonDamageTimeLeft);
	
	UFUNCTION()
		void SetConstrictionStatus(AController* Enemy, float Damage, float constrictionStatusTimeLeft, float constrictionDamageTimeLeft); 

	UFUNCTION()
		void SetBurnStatus(AController* Enemy, float Damage);
	UFUNCTION(Category = "Pawn|CustomCharacter", BlueprintCallable)
		virtual void LaunchCharacterRotated(FVector LaunchVelocity, bool bHorizontalOverride, bool bVerticalOverride);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void MakeRoll();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ConceptCharacter")
		void ToggleAutoAim();


	void GravityRotatorCalculator(float DeltaSeconds);
	void AbilityTestInUse(int32 index);
	virtual void Jump() override;
	void Dash(int32 DashDirection);
	void InverseDash();
	void CantShootDamage();
	void RestoreAbleToSoot();
	void EndStareStatus();
	void EndStunStatus();
	void EndDisruptStatus();
	void EndConstrictionStatus();
    void EndSlowStatus();

	void CCStatusUpdater(float SlowAmount = 1);

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "ConceptCharacter")
		bool AbleToMove;
	UPROPERTY()
		float TimeToInactive;
	UPROPERTY(replicated, EditInstanceOnly, BlueprintReadWrite, Category = "ConceptCharacter")
		TArray<int32> WeaponToChange;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "ConceptCharacter")
		bool IsClimbing = false;
	UPROPERTY(Category = "ConceptCharacter", EditAnywhere)
		TSubclassOf<class UCameraShake> OnSprintShakeClass;
	UPROPERTY(Category = "ConceptCharacter", EditAnywhere)
		TSubclassOf<class UCameraShake> OnDamageShakeClass;
	UPROPERTY(Replicated, EditInstanceOnly, BlueprintReadWrite, Category = "ConceptCharacter")
		int32 IndexOfAbilityInUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConceptCharacter")
		TSubclassOf<class AAbility> Ab1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConceptCharacter")
		TSubclassOf<class AAbility> Ab2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConceptCharacter")
		TSubclassOf<class AAbility> Ab3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConceptCharacter")
		TSubclassOf<class AAbility>	Ab4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConceptCharacter")
		TSubclassOf<class AAbility>	Ab5;

	UPROPERTY()
		float CameraSpeedParam;
	UPROPERTY()
		float FreeCameraSpeedParam;
	UPROPERTY()
		float TurnRateOfCharacter;
	UPROPERTY()
		FVector2D CharacterScreenBase2DLocation;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "character")
		TEnumAsByte<EOrientationToEnum::OrientedTo> OrientedTo;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "character")
		TEnumAsByte<ELookingToEnum::LookingTo> LookingToSided;
	UPROPERTY(replicated, VisibleInstanceOnly, blueprintreadonly, Category = "character")
		AAbility* CharacterSpecialAbility;
	UPROPERTY(replicated, EditInstanceOnly, blueprintreadwrite, Category = "character")
		float CharacterOpacity;



	UFUNCTION()
		void UpdateCameraOffSet();
	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	UFUNCTION()
		virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void PossessedBy(AController* NewController) override;
	UFUNCTION()
		virtual void BeginPlay() override;
	UFUNCTION()
		virtual FVector GetPawnViewLocation() const override;
	UFUNCTION()
		virtual void PostNetReceiveLocationAndRotation() override;
	UFUNCTION()
		void InitState();
	UFUNCTION()
		void InitOwnState();
	UFUNCTION()
		void InitStrangerState();
	UFUNCTION()
		void InitMouse();
	UFUNCTION()
		void InitGameAttributes();
	UFUNCTION()
		void UsePowerUp();
	UFUNCTION(BlueprintCallable, Category = "HoverSystem")
		void PlayerStartHover();
	UFUNCTION(BlueprintCallable, Category = "HoverSystem")
		void PlayerExitHover();
	UFUNCTION(BlueprintCallable, Category = "ConceptCharacter")
		void OnSprint(bool ShouldSprint);
	UFUNCTION()
		void UpdateCameraOffSetWhenIsFree();
	UFUNCTION(BlueprintCallable, Category = "Camera")
		void MoveCameraWithFreedom(bool ShouldMove);
	UFUNCTION()
		void SetIsPlayingMontage(bool State);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void SetLookingTo_Multicast(ELookingToEnum::LookingTo Looking);
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category="ConceptCharacter")
		void SetLookingTo(ELookingToEnum::LookingTo Looking);
	UFUNCTION(Server, Reliable, WithValidation)
		void SetCapsuleCollisionResponseOnServer(UPrimitiveComponent* Primitive, ECollisionChannel Channel, ECollisionResponse NewResponse);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void SetCapsuleCollisionResponseOnMulticast(UPrimitiveComponent* Primitive, ECollisionChannel Channel, ECollisionResponse NewResponse);
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "ConceptCharacter")
		void StaminaRegenerationSystem();
	UFUNCTION(Client, Unreliable)
		void StaminaExpenseSystem();
	UFUNCTION()
		void LerpControlRotation(FRotator Rotation);
	UFUNCTION()
		void ZeroGravityRotatorCalculator(float DeltaSeconds);
	UFUNCTION()
		void UpdateStatus(float DeltaSeconds);

	FVector2D LastMousePosition;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConceptCharacter")
    float LastMovementY;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConceptCharacter")
    float LastMovementX;

private:
    
	FVector2D AimVector2D;

	FVector DistanceOfAiming;

	float LastMovementYLerped;

	float LastMovementXLerped;

	void CameraManager();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginDestroy() override;

	FVector2D LastRotation;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	template<typename T>
	T* GetSpecificComponent(TArray<T*> &Components, FString ComponentName);
	FRotator RotationLerp(FRotator InitRotation, FRotator FinalRotation, float alpha);


	FVector Gravity;
	FRotator LeftOrientation;
	FRotator RightOrientation;
	FRotator FloorOrientation;
	float RotationSpeed = 1.8f;
	float VelocityMagnitude;
	float DirectionZeroG;

	FVector ZeroGravitySpeed;
	FRotator PreviousZeroGRotation;
	FRotator ZeroGravityRotation;
	FVector PreviousGravity;

	/*--------STATUS PARAMETERS AND FUNCTIONS--------*/
	void ResetStatus();

	//Poison:
	void UpdatePoison(float DeltaTime);

	AController* PoisonEnemy;
	float PoisonStatusTimeLeft;
	float PoisonStatusTimer;
	float PoisonDamageTimeLeft;
	float PoisonDamageTimer;
	float PoisonDamage;

	//Burn
	void UpdateBurn(float DeltaTime);

	AController* BurnEnemy;
	float BurnStatusTimeLeft;
	float BurnStatusTimer;
	float BurnDamageTimeLeft;
	float BurnDamageTimer;
	float BurnDamage;

	//Constriction
	void UpdateConstriction(float DeltaTime);

	AController* ConstrictionEnemy;
	float ConstrictionStatusTimeLeft;
	float ConstrictionStatusTimer;
	float ConstrictionDamageTimeLeft;
	float ConstrictionDamageTimer;
	float ConstrictionDamage;
};
