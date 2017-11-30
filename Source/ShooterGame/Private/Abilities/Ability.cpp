

#include "ShooterGame.h"
#include "Ability.h"
#include "Projectile.h"
#include "D_AssaultWeapon.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "ConceptCharacter.h"


AAbility::AAbility(){

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	RootScene2 = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene2"));
	RootScene2->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);

	CenterOfAbility = CreateDefaultSubobject<UArrowComponent>(TEXT("CenterOfAbility"));
	CenterOfAbility->AttachToComponent(RootScene2, FAttachmentTransformRules::KeepRelativeTransform);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	SkeletalMeshComponent->AttachToComponent(RootScene2, FAttachmentTransformRules::KeepRelativeTransform);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->PrimaryComponentTick.bStartWithTickEnabled = false;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->AttachToComponent(RootScene2, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleComponent->SetConstraintMode(EDOFMode::YZPlane);
	
	IsFiring = false;
	StateOfAbility = EStateOfAbility::RE_READY;
	bReplicates = true;
	AbilityMontage = NULL;
	VelocityMultiplierOnUse = 1.0f;
	Cadency = 1.0f;
	Expense = 1;
	TotalAmmo = 300;
	ClipSize = 30;
	ReloadTime = 10;
	DamageBase = 100;
	AnimationIndex = 0;
	ShouldCanalize = false;
	ProjectileSpeed = 500;
	SkeletalMeshComponent->SetRenderCustomDepth(true);
	SkeletalMeshComponent->BoundsScale = 10;
	//SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetVisibility(true, true);
	AbilityTimers = MainTimerHandle;
	IndexOfWithWeaponAnim = 0;
	bAutoAnimationMontage = true;
	OrientedToCursor = false;
	SizeOfMuzzle = 5;
	DamageTypeClass = UD_AssaultWeapon::StaticClass();
	SocketToUse = "Weapon0";
	CanalizationTime = 2;
	bAffectsShield = true;
	NetUpdateFrequency = 20;
	NetCullDistanceSquared = 125000000.0;

	NameOfAbility = NSLOCTEXT("StaticFunction", "Ability_Name", "NombrePorDefecto");
	DescriptionOfAbility = NSLOCTEXT("StaticFunction", "Ability_Description", "Descripcion por defecto");

	InitAbility();
}



void AAbility::InitAbility(){
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMeshComponent->bCastDynamicShadow = false;
	BaseTimeToFire = Cadency;
	MaxAmmo = TotalAmmo;
	CurrentAmmo = ClipSize;
	TimeRemainingToFire = 0;
	FirePointRelativeLocation = FVector_NetQuantize10::ZeroVector;
	TypeOfFirePoint = ETypeOfFirePointEnum::RE_SOCKET;
	FirePointSocket = "Muzzle";
	SkeletalMeshComponent->LightingChannels.bChannel0 = false;
	SkeletalMeshComponent->LightingChannels.bChannel1 = true;
	SkeletalMeshComponent->LightingChannels.bChannel2 = false;
	DeductAmmo();
	PrimaryAbilityProjectiles.SetNumUninitialized(0);
	AbilityData.NameAbility = NameOfAbility;
	AbilityData.DescriptionAbility = DescriptionOfAbility;
}

void AAbility::BeginPlay() {
	Super::BeginPlay();
	StopInUseAbility();
}

AActor* AAbility::FindControllerByID(int32 ActorId){
	for (TActorIterator<AConceptCharacter> Character(GetWorld()); Character; ++Character) {
		if (Character->PlayerState->PlayerId == ActorId){
			return *Character;
		}
	}
	return NULL;
}

FVector_NetQuantize10 AAbility::GetFirePoint(USkeletalMeshComponent* SkeletalMesh){

	switch (TypeOfFirePoint)
	{
		case ETypeOfFirePointEnum::RE_RELATIVE:{
			return GetActorLocation() + FirePointRelativeLocation;
			break;
		}

		case ETypeOfFirePointEnum::RE_SOCKET:{
			return SkeletalMeshComponent->GetSocketLocation(FirePointSocket);
			break;
		}
		default:
			break;
	}

	return FVector_NetQuantize10::ZeroVector;
}

FVector_NetQuantize10 AAbility::GetPointToCalculateRotationToCursor(USkeletalMeshComponent* SkeletalMesh){
	return SkeletalMeshComponent->GetSocketLocation("UpperArm_R");//GetActorLocation();
}


void AAbility::ActivateParticle_Implementation(bool Activated){
	if (Activated){
		ParticleComponent->SetWorldLocation(GetFirePoint());
		ParticleComponent->SetTemplate(ParticleMuzzle);
		ParticleComponent->ActivateSystem();
		bIsParticleActivated = true;
	}
	else{
		ParticleComponent->DeactivateSystem();
		bIsParticleActivated = false;
	}
}

void AAbility::SetActivatedParticle_Implementation(bool Activated){
	ActivateParticle(Activated);
}

void AAbility::SpawnParticle_Implementation(bool Attached, EAttachLocation::Type AttachMode = EAttachLocation::SnapToTarget) {
	if (!ParticleMuzzle) {return;}

	if (Attached)
	{
		UParticleSystemComponent* LocalParticleComponent = UGameplayStatics::SpawnEmitterAttached(ParticleMuzzle, RootScene2, NAME_None, GetFirePoint(), SkeletalMeshComponent->GetSocketRotation("Muzzle"), AttachMode, true);
		LocalParticleComponent->SetWorldScale3D(FVector(SizeOfMuzzle, SizeOfMuzzle, SizeOfMuzzle));
		LocalParticleComponent->SetWorldLocation(GetFirePoint());
		LocalParticleComponent->SetWorldRotation(SkeletalMeshComponent->GetSocketRotation("Muzzle"));
	}
	else
	{
		UParticleSystemComponent* LocalParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleMuzzle, GetFirePoint(), SkeletalMeshComponent->GetSocketRotation("Muzzle"), true);
		LocalParticleComponent->SetWorldScale3D(FVector(SizeOfMuzzle, SizeOfMuzzle, SizeOfMuzzle));
	}
}

void AAbility::ServerSpawnParticle_Implementation(bool Attached, EAttachLocation::Type AttachMode = EAttachLocation::SnapToTarget) {
	SpawnParticle(Attached, AttachMode);
}

void AAbility::ServerSpawnProjectile_Implementation(FRotator Rotator) {
	SpawnProjectile(Rotator);
}

void AAbility::SyncronizingAimRotation_Implementation(FRotator StartRotation) {

	//AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	//if(MyCharacter) {
	//	FVector XAxe, YAxe, ZAxe;
	//	FRotator FinalRotation = FRotator(StartRotation.Roll + MyCharacter->CurrentFloorOrientation.Roll, -90, 0);
	//	FRotationMatrix R(FinalRotation);
	//	R.GetScaledAxes(XAxe, YAxe, ZAxe);
	//	MyCharacter->SetLinearVectorToAim(FinalRotation);
	//}




	//FVector XAxe, YAxe, ZAxe;
	//FRotator FinalRotation = FRotator(StartRotation.Roll + MyCharacter->CurrentFloorOrientation.Roll, -90, 0);
	//FRotationMatrix R(FinalRotation);
	//R.GetScaledAxes(XAxe, YAxe, ZAxe);
	//MyCharacter->LocalLinearRotationToAim = ZAxe.Y * 90;
	//MyCharacter->SetLinearRotationToAim(ZAxe.Y * 90);
}

AProjectile* AAbility::SpawnProjectile(FRotator Rotator){
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.bNoFail = true; 

	LastProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, GetFirePoint(), Rotator, SpawnParams);
	if (LastProjectile) {
		LastProjectile->AbilityData = AbilityData;
		LastProjectile->ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(ProjectileSpeed,0,0));
		LastProjectile->bAffectsShield = bAffectsShield;
		LastProjectile->DamageBase = DamageBase;
		//AddProjectileToArray(LastProjectile);

		if (LastProjectile) {
			OnProjectileSpawned();
		}
	}

	return LastProjectile;
}

void AAbility::UseAbility(){
	UseAbilityOverridableLogic();
}

void AAbility::UseAbilityOverridableLogic_Implementation() {

}

void AAbility::PerformSpecialBehavour() {

}

void AAbility::StartInUseAbility_Implementation() {
	if (OrientedToCursor) {
		AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
		if (MyCharacter)
		{
			MyCharacter->SetOrientationToMovement_Server(!OrientedToCursor);
		}
	}
}
//
void AAbility::StopInUseAbility_Implementation() {
	if (OrientedToCursor) {
		AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
		if (MyCharacter)
		{
			MyCharacter->SetOrientationToMovement_Server(true);
		}
	}
}


void AAbility::StartAbility(){
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	if (StateOfAbility == EStateOfAbility::RE_READY){
		if(ShouldCanalize){
			CanalizeDelegate = FTimerDelegate::CreateUObject(this, &AAbility::FireAbility);
			GetWorldTimerManager().SetTimer(CanalizeHandle, CanalizeDelegate, CanalizationTime, false);
		}
		else{
			FireAbility();
		}
	}

}

void AAbility::FireAbility(){
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	if (!MyCharacter) { return; }
	if(!MyCharacter->bWantsFire){return;}

	StateOfAbility = EStateOfAbility::RE_INUSE;
	
	if (bAutoAnimationMontage && AbilityMontage) {
		MyCharacter->AbilityOfMontage = this;
		AbilityMontage_Play_Server();
	}
	else
	{
		UseAbility();
	}

	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AAbility::CheckIfIsFiring);
	GetWorldTimerManager().SetTimer(AbilityTimers, Delegate, Cadency, false);
}

void AAbility::OnAnimationEnd_Implementation(){
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());

	if(StateOfAbility == EStateOfAbility::RE_INUSE){
		StateOfAbility = EStateOfAbility::RE_RECOVERING;
	}
	
}

void AAbility::CheckIfIsFiring(){
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	if (MyCharacter && !MyCharacter->bWantsFire){
		StateOfAbility = EStateOfAbility::RE_READY;
		SetActivatedParticle(false);
	}
	else if(MyCharacter->AbilityInUse == this){
		StateOfAbility = EStateOfAbility::RE_READY;
		StartAbility();
	}
	else{
		StateOfAbility = EStateOfAbility::RE_READY;
		SetActivatedParticle(false);
	}
}

void AAbility::StopAbility() {
	StopAbilityEvent();
}

void AAbility::StopAbilityEvent_Implementation() {

}

bool AAbility::IsAbleToFire() {
	return true;
}

void AAbility::Montage_Play_Server_Implementation(UAnimMontage* Montage, float RelativeTime, bool FillTime){
	Montage_Play_Multicast(Montage, RelativeTime, FillTime);
}

void AAbility::Montage_Play_Multicast_Implementation(UAnimMontage* Montage, float RelativeTime, bool FillTime){
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());

	
	if(MyCharacter && Montage) {
		if (FillTime) {
			MyCharacter->GetMesh()->AnimScriptInstance->Montage_Play(Montage, (Montage->SequenceLength / RelativeTime));
		}
		else {
			MyCharacter->GetMesh()->AnimScriptInstance->Montage_Play(Montage, Montage->SequenceLength< RelativeTime ? 1.0f : AbilityMontage->SequenceLength  / RelativeTime);
		}
	}
}

void AAbility::AbilityMontage_Play_Server_Implementation() {AbilityMontage_Play_Multicast();}

void AAbility::AbilityMontage_Play_Multicast_Implementation() {
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(GetOwner());
	if (MyCharacter && AbilityMontage) {
		MyCharacter->GetMesh()->AnimScriptInstance->Montage_Play(AbilityMontage, (AbilityMontage->SequenceLength / AbilityMontageRate) < Cadency ? 1.0f : (AbilityMontage->SequenceLength / AbilityMontageRate) / Cadency);
	}
}

void AAbility::DeductAmmo(){
	int32 NewAmmoToCharge = (ClipSize - CurrentAmmo);
	TotalAmmo -= NewAmmoToCharge;
	CurrentAmmo += NewAmmoToCharge;
	TotalAmmo = FMath::Clamp(TotalAmmo, 0, 10000);
	CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, ClipSize);
}


void AAbility::TimeRemainingManager(float DeltaSeconds){

	
		TimeRemainingToFire = FMath::Clamp(GetWorldTimerManager().GetTimerRemaining(AbilityTimers), 0.f, 1000.f);
	
}

bool AAbility::CapsuleTraceMulti(UObject* WorldContextObject, const FVector_NetQuantize10 Start, const FVector_NetQuantize10 End, float Radius, float HalfHeight, ECollisionChannel TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, TArray<FHitResult>& OutHits, bool bIgnoreSelf){
	static const FName CapsuleTraceMultiName(TEXT("CapsuleTraceMulti"));

	FCollisionQueryParams Params(CapsuleTraceMultiName, bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.bTraceAsyncScene = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	if (bIgnoreSelf)
	{
		AActor* IgnoreActor = Cast<AActor>(WorldContextObject);
		if (IgnoreActor)
		{
			Params.AddIgnoredActor(IgnoreActor);
		}
		else
		{
			// find owner
			UObject* CurrentObject = WorldContextObject;
			while (CurrentObject)
			{
				CurrentObject = CurrentObject->GetOuter();
				AActor* IgnoreActor = Cast<AActor>(CurrentObject);
				if (IgnoreActor)
				{
					Params.AddIgnoredActor(IgnoreActor);
					break;
				}
			}
		}
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	bool const bHit = World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(Radius), Params);

	//if (true)
	//{
	//	bool bPersistent = true;//DrawDebugType == EDrawDebugTrace::Persistent;
	//	float LifeTime = 1.0f;//(DrawDebugType == EDrawDebugTrace::ForDuration) ? KISMET_TRACE_DEBUG_DRAW_DURATION : 0.f;


	//	if (bHit && OutHits.Last().bBlockingHit)
	//	{
	//		// Red up to the blocking hit, green thereafter
	//		FVector_NetQuantize10 const BlockingHitPoint = OutHits.Last().Location;
	//		::DrawDebugCapsule(World, Start, HalfHeight, Radius, FQuat::Identity, FColor::Red, bPersistent, LifeTime);
	//		::DrawDebugCapsule(World, BlockingHitPoint, HalfHeight, Radius, FQuat::Identity, FColor::Red, bPersistent, LifeTime);
	//		::DrawDebugLine(World, Start, BlockingHitPoint, FColor::Red, bPersistent, LifeTime);

	//		::DrawDebugCapsule(World, End, HalfHeight, Radius, FQuat::Identity, FColor::Green, bPersistent, LifeTime);
	//		::DrawDebugLine(World, BlockingHitPoint, End, FColor::Green, bPersistent, LifeTime);
	//	}
	//	else
	//	{
	//		// no hit means all red
	//		::DrawDebugCapsule(World, Start, HalfHeight, Radius, FQuat::Identity, FColor::Red, bPersistent, LifeTime);
	//		::DrawDebugCapsule(World, End, HalfHeight, Radius, FQuat::Identity, FColor::Red, bPersistent, LifeTime);
	//		::DrawDebugLine(World, Start, End, FColor::Red, bPersistent, LifeTime);
	//	}

	//	 //draw hits
	//	for (int32 HitIdx = 0; HitIdx < OutHits.Num(); ++HitIdx)
	//	{
	//		FHitResult const& Hit = OutHits[HitIdx];
	//		::DrawDebugPoint(World, Hit.ImpactPoint, 16.f, (Hit.bBlockingHit ? FColor::Red : FColor::Green), bPersistent, LifeTime);
	//	}
	//}

	return bHit;
}

void AAbility::UpdateProjectilesArrays()
{
	//TODO: borrar proximamente
	//for (AProjectile* Projectile: PrimaryAbilityProjectiles)
	//{
	//	if (Projectile && Projectile->RemoveFromArray())
	//	{
	//		PrimaryAbilityProjectiles.Remove(Projectile);
	//	}
	//}
}

void AAbility::AddProjectileToArray(AProjectile* Projectile)
{
		PrimaryAbilityProjectiles.Add(Projectile);
}

void AAbility::PlaySound() {
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundOnFire, GetActorLocation(), FRotator::ZeroRotator, 1.0f);
}

void AAbility::PlaySound_Server_Implementation() {
	PlaySound_Multicast();
}


void AAbility::PlaySound_Client_Implementation() {
	PlaySound();
}

void AAbility::PlaySound_Multicast_Implementation() {
	PlaySound();
}


void AAbility::Tick_Implementation(float DeltaSeconds){
	Super::Tick(DeltaSeconds);
	TimeSinceLastShot += DeltaSeconds;
	TimeRemainingManager(DeltaSeconds);
	//UpdateProjectilesArrays();
}


bool AAbility::ServerSpawnProjectile_Validate(FRotator Rotator) { return true; }

bool AAbility::SetActivatedParticle_Validate(bool Activated){ return true; }

bool AAbility::ActivateParticle_Validate(bool Activated){ return true; }

bool AAbility::SpawnParticle_Validate(bool Attached, EAttachLocation::Type AttachMode = EAttachLocation::SnapToTarget){ return true; }

bool AAbility::Montage_Play_Server_Validate(UAnimMontage* Montage, float RelativeTime, bool FillTime){ return true; }

bool AAbility::Montage_Play_Multicast_Validate(UAnimMontage* Montage, float RelativeTime, bool FillTime){ return true; }

bool AAbility::AbilityMontage_Play_Server_Validate() { return true; }

bool AAbility::AbilityMontage_Play_Multicast_Validate() { return true; }

bool AAbility::ServerSpawnParticle_Validate(bool Attached, EAttachLocation::Type AttachMode = EAttachLocation::SnapToTarget) {return true;}

bool AAbility::PlaySound_Server_Validate() { return true; }

bool AAbility::PlaySound_Client_Validate() { return true; }

bool AAbility::PlaySound_Multicast_Validate() { return true; }

