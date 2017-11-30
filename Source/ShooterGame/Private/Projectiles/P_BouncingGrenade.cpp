// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "P_BouncingGrenade.h"


AP_BouncingGrenade::AP_BouncingGrenade(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	//static ConstructorHelpers::FObjectFinder<USoundCue> Sonido(TEXT("/Game/Audio/Explosion_Cue.Explosion_Cue"));
	//SonidoExplosion = Sonido.Object;

}

void AP_BouncingGrenade::BeginPlay() {
	Super::BeginPlay();
	FTimerHandle Handle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AP_BouncingGrenade::ExplodeEmitter);
	GetWorldTimerManager().SetTimer(Handle, TimerDelegate, 4.0f, false);
}

void AP_BouncingGrenade::ProjectileEffect(AActor* ActorToAffectParam, const FHitResult& Hit) {
	APawn* Enemy = Cast<APawn>(ActorToAffectParam);
	if (Enemy) {
		GrenadeExplode(Hit);
	}
}

void AP_BouncingGrenade::GrenadeExplode(const FHitResult& Hit) {

	SpawnExplosionEffect();
	SpawnExplosionDecal(Hit);

	APawn* Character = Cast<APawn>(GetOwner());
	if (Character) {
		UGameplayStatics::ApplyRadialDamage(this, DamageBase, GetActorLocation(), EffectArea, DamageType, TArray<AActor*>(), this, (AController*)Character->Controller, true, ECollisionChannel::ECC_GameTraceChannel5);
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SonidoExplosion, GetActorLocation(), 2.0);
	Destroy();
}


void AP_BouncingGrenade::ExplodeEmitter_Implementation() {
	FHitResult Hit;
	Hit.Location = this->GetActorLocation();
	GrenadeExplode(Hit);
}

void AP_BouncingGrenade::Explode_Implementation() { ExplodeEmitter(); }

bool AP_BouncingGrenade::Explode_Validate() {return true;}