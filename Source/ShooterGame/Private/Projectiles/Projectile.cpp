#include "ShooterGame.h"
#include "MinionCharacter.h"
#include "Projectile.h"
#include "D_AssaultWeapon.h"
#include "ConceptPlayerState.h"


AProjectile::AProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;


	//static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMaterial1(TEXT("MaterialInstanceConstant'/Game/Materials/Instances/MI_BloodDecal2.MI_BloodDecal2'"));
	//BloodDecalMaterials.Add(BloodMaterial1.Object);

	//static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMaterial2(TEXT("MaterialInstanceConstant'/Game/Materials/Instances/MI_BloodDecal2.MI_BloodDecal2'"));
	//BloodDecalMaterials.Add(BloodMaterial2.Object);

	//static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMaterial3(TEXT("MaterialInstanceConstant'/Game/Materials/Instances/MI_BloodDecal2.MI_BloodDecal2'"));
	//BloodDecalMaterials.Add(BloodMaterial3.Object);

	//static ConstructorHelpers::FObjectFinder<UMaterial> ImpactMaterial(TEXT("Material'/Game/Materials/EspecificMaterials/M_ImpactDecal_RifleNHC.M_ImpactDecal_RifleNHC'"));
	//ImpactDecal = ImpactMaterial.Object;

	//Material'/Game/Materials/EspecificMaterials/M_ImpactDecal.M_ImpactDecal'

	CollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollisionComponent"));
	ProjectileMovementComponent = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileMovementComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->OnComponentHit.AddDynamic( this, &AProjectile::OnHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;

	CollisionComponent->AlwaysLoadOnClient = true;
	CollisionComponent->AlwaysLoadOnServer = true;
	CollisionComponent->bTraceComplexOnMove = false;
	

	DamageBase = 5;
	SizeOfParticle = 2;
	bAffectsShield = true;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	NetUpdateFrequency = 20;
	NetCullDistanceSquared = 12500000.0;
	bAffectTeammates = false;

	bRemove = false;
}

void AProjectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
//		if (ProjectileTrail->IsValidLowLevelFast()) {
//			ProjectileTrail->SetWorldLocation(TrailLocation, true);
//			TrailLocation = CollisionComponent->GetComponentLocation();
//		}
}

void AProjectile::BeginPlay() {
	Super::BeginPlay();
	CollisionComponent->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
	if (ParticleTrail->IsValidLowLevelFast()) {
//		TrailLocation = CollisionComponent->GetComponentLocation();
//		ProjectileTrail = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleTrail, TrailLocation);
		UGameplayStatics::SpawnEmitterAttached(ParticleTrail, CollisionComponent);
	}
    UGameplayStatics::SpawnEmitterAttached(ParticleTrail, CollisionComponent);

}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	if (ProjectileTrail->IsValidLowLevelFast()) {
		ProjectileTrail->SetWorldLocation(TrailLocation, true);
		TrailLocation = CollisionComponent->GetComponentLocation();
		ProjectileTrail->Deactivate();
	}
}

//fuego amigo sin colision
void AProjectile::OnHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	AConceptCharacter* Character = Cast<AConceptCharacter>(OtherActor);
	AConceptCharacter* SelfCharacter = Cast<AConceptCharacter>(GetOwner());
	FirstHit = Hit;
	
	PlaySound_Server();
	
	if (!OnCharacterHittedEffect(Hit)) {
		SpawnNormalImpactDecal(Hit);
		SpawnHitEffect(Hit);
	}

	if (Character && SelfCharacter && SelfCharacter == Character) {return;}
	if (OtherComp && OtherComp->ComponentHasTag(FName(TEXT("Shield"))) && ProjectileMovementComponent->bShouldBounce) { return; }

	ProjectileEffect(OtherActor, Hit);
	bRemove = true;
}

void AProjectile::OnOverlap(UPrimitiveComponent* MyComp, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
	if (!GetWorld()->GetFirstPlayerController()) return;
	if (OtherComponent->ComponentHasTag(FName(TEXT("Gravity")))){ return; }
	if (IsValidEnemy(OtherActor)){
		UStaticFunctionLibrary::PrintString(this, "valid enemy");
		SpawnHitEffect(SweepResult);
		ProjectileEffect(OtherActor, SweepResult);
		bRemove = true;
	}
}

void AProjectile::ProjectileEffect( AActor* ActorToAffectParam, const FHitResult& Hit) {
}

void AProjectile::EffectLogic(AActor* ActorToAffectParam, const FHitResult& Hit) {
}

void AProjectile::EffectVisual(AActor* ActorToAffectParam, const FHitResult& Hit) {
}

void AProjectile::EffectVisual_Server_Implementation(AActor* ActorToAffectParam, const FHitResult& Hit) {EffectVisual_Multicast(ActorToAffectParam, Hit);}
bool AProjectile::EffectVisual_Server_Validate(AActor* ActorToAffectParam, const FHitResult& Hit) {return true;}
void AProjectile::EffectVisual_Multicast_Implementation(AActor* ActorToAffectParam, const FHitResult& Hit) { EffectVisual(ActorToAffectParam, Hit); }
bool AProjectile::EffectVisual_Multicast_Validate(AActor* ActorToAffectParam, const FHitResult& Hit) { return true; }


void AProjectile::SpawnNormalImpactDecal(const FHitResult &Hit){
	if (ImpactDecal == NULL) { return; }

	FVector Location = Hit.Location;
	FVector ImpactNormal = Hit.ImpactNormal;

	float RandomSize = FMath::FRandRange(-10, 10);
	FVector RandomLocationOffset = FVector(FMath::FRandRange(-50, 50), 0, 0);
	FVector RandomSizeOffset = FVector(0, RandomSize, RandomSize);
	UDecalComponent* LocalDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactDecal->GetMaterial(), FVector(40, 25, 25) + RandomSizeOffset, Location + RandomLocationOffset, (ImpactNormal * -1).Rotation(), 3);
	if(LocalDecal){
		LocalDecal->SetWorldLocation(Location + RandomLocationOffset);
		LocalDecal->SetWorldRotation((ImpactNormal * -1).Rotation());
		LocalDecal->FadeScreenSize = 0.001;
		LocalDecal->SetFadeOut(2, 5);
	}
}

void AProjectile::SpawnExplosionDecal(const FHitResult &Hit) {
	FVector Location = Hit.Location;
	FVector ImpactNormal = Hit.ImpactNormal;

	float RandomSize = FMath::FRandRange(-10, 10);
	FVector RandomLocationOffset = FVector(FMath::FRandRange(-50, 50), 0, 0);
	FVector RandomSizeOffset = FVector(0, RandomSize, RandomSize);
	UDecalComponent* LocalDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactDecal->GetMaterial(), FVector(40, SizeOfDecal, SizeOfDecal) + RandomSizeOffset, Location + RandomLocationOffset, (ImpactNormal * -1).Rotation(), 3);
	if(LocalDecal){
		LocalDecal->SetWorldLocation(Location + RandomLocationOffset);
		LocalDecal->SetWorldRotation((ImpactNormal * -1).Rotation());
		LocalDecal->FadeScreenSize = 0.001;
		LocalDecal->SetFadeOut(6, 10);
	}
}

void AProjectile::SpawnHitEffect(const FHitResult &Hit){
	if (ProjectileTrail->IsValidLowLevelFast()) {
		ProjectileTrail->SetWorldLocation(Hit.Location);
		TrailLocation = Hit.Location;
	}

	AConceptCharacter* Character = Cast<AConceptCharacter>(Hit.GetActor());



	if (ParticlesOnHit.Num() > 0) {
		if (Character) {
			UParticleSystemComponent* SpawnedParticleBlood = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticlesOnHit[0], Hit.Location);
			if (SpawnedParticleBlood) {
				SpawnedParticleBlood->SetWorldScale3D(FVector(6, 6, 6));
				SpawnedParticleBlood->SetBoundsScale(10.0f);
			}
		}

		UParticleSystemComponent* SpawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticlesOnHit[1], Hit.Location);
		if (SpawnedParticle) {
			SpawnedParticle->SetWorldScale3D(FVector(4, 4, 4));
			SpawnedParticle->SetBoundsScale(10.0f);
		}
	}
}

void AProjectile::SpawnExplosionEffect() {
	if (ExplosionParticle) {
		UParticleSystemComponent* SpawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, this->GetActorLocation());
		if (SpawnedParticle) {
			SpawnedParticle->SetWorldScale3D(FVector(SizeOfParticle, SizeOfParticle, SizeOfParticle));
			SpawnedParticle->SetBoundsScale(10.0f);
		}
	}
}

bool AProjectile::OnCharacterHittedEffect(const FHitResult &Hit)
{
	AConceptCharacter* Character = Cast<AConceptCharacter>(Hit.GetActor());
	if (Character) {

		//FVector RandomOffset = FVector(FMath::FRandRange(-1, 1), FMath::FRandRange(-1, 1), FMath::FRandRange(-1, 1));
		//FVector NormalWithRandomOffset = FVector(Hit.ImpactNormal.X + RandomOffset.X, Hit.ImpactNormal.Y + RandomOffset.Y, Hit.ImpactNormal.Z + RandomOffset.Z);
		//NormalWithRandomOffset.Normalize();

		//TArray<FHitResult> Hits;
		//TArray<AActor*> ActorsToIgnore;
		//ActorsToIgnore.Add(Character);
		//UStaticFunctionLibrary::LineTraceMulti(GetWorld(), Hit.Location, FVector(Hit.Location.X + NormalWithRandomOffset.X * 500, Hit.Location.Y + NormalWithRandomOffset.Y * 500, Hit.Location.Z + NormalWithRandomOffset.Z * 500), ECollisionChannel::ECC_WorldStatic, false, ActorsToIgnore, Hits, true);

		//for (int32 i = 0; i < Hits.Num(); i++)
		//{

		//	AAbility* Ability = Cast<AAbility>(Hits[i].GetActor());
		//	if (!Ability)
		//	{
		//		float Diference = (Hits[i].Location - Character->GetActorLocation()).Size();

		//		float RandomScale = DamageBase * (2000 / FMath::Abs(Diference));
		//		RandomScale = FMath::Clamp(RandomScale, 5.f, 60.f);

		//		UDecalComponent* LocalDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BloodDecalMaterials[FMath::RandRange(0, BloodDecalMaterials.Num() - 1)]->GetMaterial(), FVector(40, RandomScale, RandomScale), Hits[i].Location, (Hits[i].ImpactNormal * -1).Rotation(), RandomScale * 2);
		//		LocalDecal->SetWorldLocation(Hits[i].Location);
		//		LocalDecal->SetWorldRotation((Hits[i].ImpactNormal * -1).Rotation());
		//	}
		//}
	}
	else
	{
		return false;
	}

	return true;
}

bool AProjectile::IsValidEnemy(AActor* OtherActor) {
	AConceptCharacter* CharacterTarget = Cast<AConceptCharacter>(OtherActor);
	//AMinionCharacter* MinionTarget = Cast<AMinionCharacter>(OtherActor);
	AController* MyController = Cast<AController>(GetOwner());
	AConceptCharacter* MyCharacter = Cast<AConceptCharacter>(MyController ? MyController->GetPawn() : GetOwner());
    if(!MyCharacter) return true;
    
	int32 MyTeam = 0, TargetTeam = 3;
	if (MyCharacter && MyCharacter->MyPlayerState){
		MyTeam = MyCharacter->MyPlayerState->Team;
	}
	if (CharacterTarget && CharacterTarget->MyPlayerState){
		TargetTeam = CharacterTarget->MyPlayerState->Team;
	}

	return (MyTeam != TargetTeam || (MyCharacter != CharacterTarget && bAffectTeammates));
}


bool AProjectile::RemoveFromArray()
{
	return bRemove;
}

void AProjectile::PlaySound() {UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundOnHit, GetActorLocation(), FRotator::ZeroRotator, 1.0f);}

void AProjectile::PlaySound_Server_Implementation() {PlaySound_Multicast();}

void AProjectile::PlaySound_Client_Implementation() {PlaySound();}

void AProjectile::PlaySound_Multicast_Implementation() {PlaySound();}

bool AProjectile::PlaySound_Server_Validate() { return true; }

bool AProjectile::PlaySound_Client_Validate() { return true; }

bool AProjectile::PlaySound_Multicast_Validate() { return true; }
