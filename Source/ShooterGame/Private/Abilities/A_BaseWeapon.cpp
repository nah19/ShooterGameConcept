// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "ConceptPlayerController.h"
#include "A_BaseWeapon.h"
#include "D_AssaultWeapon.h"


AA_BaseWeapon::AA_BaseWeapon(){
	PrimaryActorTick.bCanEverTick = true;


	ProjectileSpeed = 1500;
	DistanceOfEffectContinuous = 1000;
	RadiusOfEffectContinuous = 10;
	AbilityData.TypeOfAbility = ETypeOfAbilityEnum::RE_WEAPON;
	TypeOfWeapon = ETypeOfWeapon::RE_NORMAL;
	Desviation = 0;
	ShotsPerBurst = 4;
	ReloadTime = 10;
	InternalBurstCadency = 0.1f;
	TimeJammed = 5.0f;
	IsJammed = false;
	ContinuousDamage = 10;
	IsFastProjectile = false;
	WeaponMeleeHitBox = NULL;
	WeaponMeleeStaminaWaste = 10;


	InitWeapon();
}

void AA_BaseWeapon::InitWeapon(){
	InitAbility();
	SkeletalMeshComponent->bCastDynamicShadow = false;
	IsReloading = false;
	CurrentShotInBurst = ShotsPerBurst;
}

void AA_BaseWeapon::UseAbility(){
	if (TypeOfWeapon == ETypeOfWeapon::RE_CONTINUOUS){
		Fire();
	}
	else{
		Super::UseAbility();
		Fire();
	}
}

void AA_BaseWeapon::Fire_Implementation(){
	if (HasCurrentAmmo()){
			switch (TypeOfWeapon){
				case ETypeOfWeapon::RE_NORMAL:{
					NormalShot();
					TimeRemainingToFire = Cadency;
					BaseTimeToFire = Cadency;
				}break;
				case ETypeOfWeapon::RE_BURST:{
                    ServerSpawnParticle(true);
					CurrentShotInBurst = ShotsPerBurst;
					BurstShoot();
					TimeRemainingToFire = Cadency;
					BaseTimeToFire= Cadency;
				}break;
				case ETypeOfWeapon::RE_CONTINUOUS:{
					ContinuousShot();
					BaseTimeToFire = 10;
				}break;
				case ETypeOfWeapon::RE_INSTANTBURST:
					InstantBurstShoot();
					TimeRemainingToFire = Cadency;
					BaseTimeToFire = Cadency;
					break;
				case ETypeOfWeapon::RE_MELEE:
					break;
			}
		//}
	}
	else{
		StartReload();
	}

}

void AA_BaseWeapon::Shot_Implementation(FRotator FinalRotation){

	if (IsFastProjectile){
		ShotReplicated(FinalRotation);
	}else {
		AProjectile* ProjectileSpawned = SpawnProjectile(FinalRotation);
		LastProjectile = ProjectileSpawned;
		if (TypeOfWeapon != ETypeOfWeapon::RE_BURST && TypeOfWeapon != ETypeOfWeapon::RE_INSTANTBURST && TimeSinceLastShot > 0.1) {
			TimeSinceLastShot = 0;
			SpawnParticle(true);
		}
	}
}

void AA_BaseWeapon::ShotReplicated_Implementation(FRotator FinalRotation){
	AProjectile* ProjectileSpawned = SpawnProjectile(FinalRotation);
	if (TypeOfWeapon != ETypeOfWeapon::RE_BURST && TypeOfWeapon != ETypeOfWeapon::RE_INSTANTBURST && TimeSinceLastShot > 0.1) {
		TimeSinceLastShot = 0;
		SpawnParticle(true);
	}
}


void AA_BaseWeapon::NormalShot_Implementation(){
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	AConceptPlayerController* MyPlayerController = Cast<AConceptPlayerController>(MyCharacter->GetController());
	if (!MyPlayerController) { return; }

	CurrentAmmo -= Expense;
	CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, 10000);


	FRotator RotationToSpawn = MyPlayerController->CalculateRotToCursorPosition(GetPointToCalculateRotationToCursor(), 10, 0);
	float RandomizedDesviation = FMath::RandRange(-Desviation, Desviation);
	FRotator FinalRotation = FRotator(RotationToSpawn.Roll + RandomizedDesviation, -90, 0);

	Shot(FinalRotation);
	HasCurrentAmmo();
}

void AA_BaseWeapon::BurstShoot() {

	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	AConceptPlayerController* MyPlayerController = Cast<AConceptPlayerController>(MyCharacter->GetController());
	if (!MyPlayerController) { return; }

	FRotator RotationToSpawn = MyPlayerController->CalculateRotToCursorPosition(GetPointToCalculateRotationToCursor(), 10, 0);
	float RandomizedDesviation = FMath::RandRange(-Desviation, Desviation);
	FRotator FinalRotation = FRotator(RotationToSpawn.Roll + RandomizedDesviation, -90, 0);

	Shot(FinalRotation);

	CurrentAmmo -= Expense;
	CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, 10000);
	CurrentShotInBurst--;

	if (CurrentShotInBurst <= 0) {
		TimeRemainingToFire = Cadency;
		HasCurrentAmmo();
	}
	else {
		FTimerHandle Handle;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AA_BaseWeapon::BurstShoot);
		GetWorldTimerManager().SetTimer(Handle, Delegate, InternalBurstCadency, false);
	}

}

void AA_BaseWeapon::InstantBurstShoot_Implementation() {
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	AConceptPlayerController* MyPlayerController = Cast<AConceptPlayerController>(MyCharacter->GetController());
	if (!MyPlayerController) { return; }

	FRotator RotationToSpawn = MyPlayerController->CalculateRotToCursorPosition(GetPointToCalculateRotationToCursor(), 10, 0);

	for (int i = 0; i < ShotsPerBurst; i++) {
		CurrentAmmo -= Expense;
		CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, 10000);

		float RandomizedDesviation = FMath::RandRange(-Desviation, Desviation);
		FRotator FinalRotation = FRotator(RotationToSpawn.Roll + RandomizedDesviation, -90, 0);


		if (i == 1)
		{
			Shot(FinalRotation);
			ServerSpawnParticle(true);
		}
		else {
			Shot(FinalRotation);
		}
	}
	HasCurrentAmmo();
}

void AA_BaseWeapon::ContinuousShot(){
	
	if (EnoughTimeSinceLastShot()){
		GetWorldTimerManager().SetTimer(AbilityTimers, FMath::Clamp((TimeRemainingToFire + 0.1f), 0.f, 12.0f), false);

		CurrentAmmo--;
		CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, 10000);

		if (!bIsParticleActivated) {
			SetActivatedParticle(true);
		}
		AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
		MyCharacter->LerpSpeed = 0.6f;
		MyCharacter->AimRefreshRate = 0.3f;
		AConceptPlayerController* MyPlayerController = Cast<AConceptPlayerController>(MyCharacter->GetController());
		if (!MyPlayerController){ return; }
		
		FVector_NetQuantize10 Point = GetPointToCalculateRotationToCursor(0);
		FRotator RotationToSpawn = MyPlayerController->CalculateRotToCursorPosition(Point, 10, 0);
		
		FVector EndLocation;
		if (!MyPlayerController->bUseGamepad) {
			FVector	WorldLocation, WorldDirection;
			MyPlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
			EndLocation = WorldLocation + (WorldDirection * (WorldLocation.X / WorldDirection.X)) * -1;
		}
		else {
			EndLocation = MyCharacter->AimingPadVector;
			EndLocation = EndLocation.RotateAngleAxis(MyCharacter->CurrentFloorOrientation.Roll, FVector(1.0f, 0, 0));
			EndLocation *= 1000;
			EndLocation += Point;
		}

		
		FRotator FinalRotation = FRotator(0, 0, RotationToSpawn.Roll);
		
		SetParticleRotation_Server(FRotator(FinalRotation.Roll, -90, 0));
		ContinuousShotDetector(RotationToSpawn, EndLocation);
	}


	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AA_BaseWeapon::CheckIfIsFiringContinous);
	GetWorldTimerManager().SetTimer(Handle, Delegate, 0.2f, false);
}



void AA_BaseWeapon::ContinuousShotDetector_Implementation(FRotator RotationToAim, FVector_NetQuantize10 EndPos) {
	TArray<AActor*> ActorsToIgnore;
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	
	for (TActorIterator<AConceptPlayerState> PlayerStateItr(GetWorld()); PlayerStateItr; ++PlayerStateItr) {
		if(PlayerStateItr->Team == MyCharacter->MyPlayerState->Team) {
			ActorsToIgnore.Add(PlayerStateItr->MyCharacter);
		}
	}
	
	AConceptPlayerController* MyPlayerController = Cast<AConceptPlayerController>(MyCharacter->GetController());
	if (!MyPlayerController) { return; }

	FVector_NetQuantize10 Point = GetPointToCalculateRotationToCursor(0);
	FRotator RotationToSpawn = MyPlayerController->CalculateRotToCursorPosition(Point, 10, 0);

	TArray<FHitResult> OutHits;
	
	FVector_NetQuantize10 DirecVector = EndPos - GetFirePoint();
	DirecVector.Normalize();
	FVector_NetQuantize10 ForwardVector = DirecVector;

	FVector_NetQuantize10 GamepadVector = FVector_NetQuantize10(0, -1 * MyCharacter->LocalLinearVectorToAim.X, MyCharacter->LocalLinearVectorToAim.Y);

	FVector_NetQuantize10 StartPos = GetFirePoint() + ForwardVector * 20;
	FVector_NetQuantize10 FinalPos = StartPos + GamepadVector * DistanceOfEffectContinuous;

	bool IsSomethingHitted = false;

	IsSomethingHitted = CapsuleTraceMulti(GetWorld(), StartPos, FinalPos, RadiusOfEffectContinuous, 0, ECC_Visibility, false, ActorsToIgnore, OutHits, true);

	DamageObjectsHitted(IsSomethingHitted, OutHits);
}

void AA_BaseWeapon::CheckIfIsFiringContinous(){
	//AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());

	//if(MyCharacter->bWantsFire && HasCurrentAmmo()) {
	//	//ContinuousShot();
	//	return;
	//}
	//StateOfAbility = EStateOfAbility::RE_READY;
	//SetActivatedParticle(false);
	//MyCharacter->LerpSpeed = 0.3f;
	//MyCharacter->AimRefreshRate = 0.3f;

	CheckIfIsFiring();
}

bool AA_BaseWeapon::HasEnoughStamina()
{
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	if (MyCharacter)
		return MyCharacter->Stamina > WeaponMeleeStaminaWaste;
	else
		return false;
}

void AA_BaseWeapon::ReduceStamina(){
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	if (MyCharacter) {
		MyCharacter->Stamina -= WeaponMeleeStaminaWaste;
		if (MyCharacter->Stamina < 0.0f)
			MyCharacter->Stamina = 0.0f;
		MyCharacter->StartStaminaRegeneration();
	}
}



bool AA_BaseWeapon::HasCurrentAmmo(){
	if (CurrentAmmo <= 0){
		StartReload();
		return false;
	}
	return true;
}

bool AA_BaseWeapon::HasTotalAmmo(){
	if (TotalAmmo <= 0){
		TotalAmmo = 0;
		return false;
	}
	return true;
}

bool AA_BaseWeapon::EnoughTimeSinceLastShot(){
	if (TypeOfWeapon == ETypeOfWeapon::RE_CONTINUOUS){
		return (TimeRemainingToFire < 10);
	}

	return (TimeRemainingToFire <= 0.05);
}

void AA_BaseWeapon::StartReload(){
	if (StateOfAbility == EStateOfAbility::RE_RELOADING){ return; }
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	StateOfAbility = EStateOfAbility::RE_RELOADING;
	MyCharacter->AbilityOfMontage = this;
	Montage_Play_Server(ReloadingMontage, ReloadTime, true);
	BaseTimeToFire = ReloadTime;

	
	SetActivatedParticle(false);


	FTimerHandle Handle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AA_BaseWeapon::EndReload);
	GetWorldTimerManager().SetTimer(AbilityTimers, Delegate, ReloadTime, false);
}

void AA_BaseWeapon::EndReload(){
	DeductAmmo();
	BaseTimeToFire = Cadency;

	CheckIfIsFiring();
}

void AA_BaseWeapon::DamageObjectsHitted_Implementation(bool IsSomethingHitted, const TArray<FHitResult>& OutHits) {
	if (IsSomethingHitted) {
		TArray<APawn*> CharactersDamaged;
		for (int32 i = 0; i < OutHits.Num(); i++){
			FHitResult const& Hit = OutHits[i];

			AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
			AConceptPlayerController* MyPlayerController = Cast<AConceptPlayerController>(MyCharacter->GetController());
			
			if (Hit.GetActor() && Hit.GetActor()->bCanBeDamaged)
			{
				APawn* CharacterHitted = Cast<APawn>(Hit.GetActor());
				if (CharacterHitted && !CharactersDamaged.Contains(CharacterHitted)) {
					CharactersDamaged.AddUnique(CharacterHitted);
					UGameplayStatics::ApplyDamage(CharacterHitted, ContinuousDamage, (AController*)MyPlayerController, this, DamageTypeClass);
					//ParticleComponent->SetActorParameter(FName("self"), this);
					ParticleComponent->SetActorParameter(FName("Target"), CharacterHitted);
				}
			}
		}
	} else {
		//ParticleComponent->SetActorParameter(FName("self"), this);
		ParticleComponent->SetActorParameter(FName("Target"), NULL);
	}
}

void AA_BaseWeapon::SetParticleRotation_Multicast_Implementation(FRotator StartRotation) { ParticleComponent->SetWorldRotation(StartRotation); }

bool AA_BaseWeapon::SetParticleRotation_Multicast_Validate(FRotator StartRotation) { return true; }

void AA_BaseWeapon::SetParticleRotation_Server_Implementation(FRotator StartRotation) { SetParticleRotation_Multicast(StartRotation); }

bool AA_BaseWeapon::SetParticleRotation_Server_Validate(FRotator StartRotation) { return true; }

bool AA_BaseWeapon::DamageObjectsHitted_Validate(bool IsSomethingHitted, const TArray<FHitResult>& OutHits){return true;}

bool AA_BaseWeapon::ContinuousShotDetector_Validate(FRotator RotationToAim, FVector_NetQuantize10 EndPos){return true;}

bool AA_BaseWeapon::Shot_Validate(FRotator FinalRotation){ return true; }

bool AA_BaseWeapon::NormalShot_Validate(){ return true; }

bool AA_BaseWeapon::InstantBurstShoot_Validate(){ return true; }



