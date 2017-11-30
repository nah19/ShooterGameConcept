// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGame.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "MinionCharacter.h"
#include "DamageTypes/D_AssaultWeapon.h"
#include "A_TurretShoot.h"



AA_TurretShoot::AA_TurretShoot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	MaxHealth = 1000;
	Health = MaxHealth;
	TurretRange = 500;
	TurretTurnRate = 200;
}

void AA_TurretShoot::BeginPlay() {
	Super::BeginPlay();
	TArray<USceneComponent*> Scenes;
	//TArray<UDestructibleComponent*> Destructibles;
	//DestructibleTurret = GetSpecificComponent(Destructibles, "BaseTurret");
	Spawner = GetSpecificComponent(Scenes, "Scene");
	SpawnerPoint = GetSpecificComponent(Scenes, "SpawnPoint");
	Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticle, Spawner->GetComponentLocation());
	Beam->SetVectorParameter(FName(TEXT("BeamSource")), Spawner->GetComponentLocation());
	Beam->SetVisibility(false);
	FRotator BeamRotator = Beam->GetComponentRotation();
	BeamRotator.Pitch += 90;
	Beam->SetWorldRotation(BeamRotator);
}

void AA_TurretShoot::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	//AimTo(TargetLocation(), DeltaTime);
	if (TargetIsValid()){
		//TODO: attack behaviour con cadencia y tal

		AimTo(TargetLocation(), DeltaTime);
	}
	else if (Beam && Beam->bVisible) {
		Beam->SetVisibility(false);
	}
}

void AA_TurretShoot::AimTo_Implementation(FVector TargetLocation, float DeltaTime){
	if (IsValid(Spawner)){

		//FRotator lookAtTarget = (TargetLocation - Spawner->GetComponentLocation()).Rotation();
		//lookAtTarget.Pitch = 0;
		//lookAtTarget.Yaw = 0;
		//lookAtTarget.Roll = 0;
		FRotator Rotation = FRotationMatrix::MakeFromX(TargetLocation - Spawner->GetComponentLocation()).Rotator();

		//FRotator WorldLookAt = FMath::RInterpConstantTo(Spawner->K2_GetComponentRotation(), lookAtTarget, DeltaTime, TurretTurnRate);
		Spawner->SetWorldRotation(Rotation);

		if (!Target) {
			Beam->SetVisibility(false);
			return;
		}
		Beam->SetActorParameter(FName(TEXT("BeamTarget")), Target);
		Beam->SetVisibility(true);
		UGameplayStatics::ApplyDamage(Target, 0.5f, (AController*)this, this, UD_AssaultWeapon::StaticClass());
	}
}

bool AA_TurretShoot::AimTo_Validate(FVector TargetLocation, float Deltatime){
	return true;//IsValid(Spawner);
}

float AA_TurretShoot::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	float fDamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	AConceptPlayerController* Controller = Cast<AConceptPlayerController>(EventInstigator);
	if (Controller){
		AConceptCharacter* Character = Cast<AConceptCharacter>(Controller->GetPawn());
		Character->OtherHasReceivedDamage();

	}
	if ((Health -= DamageAmount) <= 0){
		Death();
	}
	return DamageAmount;
}

void AA_TurretShoot::Explode(){
	UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, this->GetActorLocation());
	Particle->SetWorldScale3D(FVector(3, 3, 3));
	APawn* Character = Cast<APawn>(GetOwner());
	if (Character) {
		UGameplayStatics::ApplyRadialDamage(this, 70.f, GetActorLocation(), 850.f, UD_AssaultWeapon::StaticClass(), TArray<AActor*>(), this, (AController*)Character->Controller);
	}
	Destroy();
}

void AA_TurretShoot::Death_Implementation() {
	Destroy();
	//FTimerHandle Handle;
	//FTimerDelegate delegate = FTimerDelegate::CreateUObject(this, &AA_TurretShoot::DelayedDestroy);
	//GetWorldTimerManager().SetTimer(Handle, delegate, 5.0f, false);

	//TODO recompensa por matar torreta??

	//DestructibleTurret->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Spawner->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//DestructibleTurret->ApplyDamage(99999.0f, DestructibleTurret->GetComponentLocation(), DestructibleTurret->GetComponentLocation(), 10.0f);
}

void AA_TurretShoot::DelayedDestroy_Implementation() {
	Destroy();
}

template <typename T>
T* AA_TurretShoot::GetSpecificComponent(TArray<T*> &Components, FString ComponentName) {
	this->GetComponents(Components);
	for (auto Component : Components){
		if (Component->GetName() == ComponentName){
			return Component;
		}
	}
	return NULL;
}

float AA_TurretShoot::DistanceToTarget(){
	return (this->GetActorLocation() - Target->GetActorLocation()).Size();
}

bool AA_TurretShoot::TargetIsValid(){
	APawn* EnemyChar = Cast<APawn>(Target);
	if (EnemyChar){
		AMinionCharacter* Minion = Cast<AMinionCharacter>(EnemyChar);
		if (Minion){
			return (IsValid(Minion) && IsValid(Spawner) && DistanceToTarget() <= TurretRange && TeamNumber != Minion->TeamNumber);
		}
		AConceptPlayerState* Enemy = Cast<AConceptPlayerState>(EnemyChar->PlayerState);
		return (IsValid(Enemy) && IsValid(Spawner) && DistanceToTarget() <= TurretRange && TeamNumber != Enemy->Team);
	}
	return false;
}

FVector AA_TurretShoot::TargetLocation(){

	//EN este punto manualcontrol deberia ser true!!!!!!!!!!!!!!!!!!!!!!

	if (!ManualControl){
		return Target->GetActorLocation();
	}
	Target = NULL;
	FVector WorldLocation, WorldDirection;
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	MyController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(MyController->GetPawn());
	if (MyCharacter) {
		Target = MyCharacter->SelectedTarget;
	}

	return WorldLocation + (WorldDirection * (WorldLocation.X / WorldDirection.X)) * -1;
}


void AA_TurretShoot::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AA_TurretShoot, ManualControl);
	DOREPLIFETIME(AA_TurretShoot, Target);

}