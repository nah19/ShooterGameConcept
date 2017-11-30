// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "P_AttractionArea.h"


AP_AttractionArea::AP_AttractionArea(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {
    PrimaryActorTick.bCanEverTick = true;
    Force = CreateDefaultSubobject<URadialForceComponent>(TEXT("Force"));
    Force->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    Force->Radius = 1500.0f;
    Force->ForceStrength = -800000.0f;
}

void AP_AttractionArea::BeginPlay(){
    Super::BeginPlay();
    if(bDeployed) return;
    FTimerHandle Handle;
    FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AP_AttractionArea::Deploy);
    GetWorldTimerManager().SetTimer(Handle, TimerDelegate, DeployTime, false);
}

void AP_AttractionArea::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    if(bDeployed){
        CollisionComponent->SetWorldLocation(DeployLocation);
    }
}

void AP_AttractionArea::Deploy(){
    bDeployed = true;
    DeployLocation = GetActorLocation();
    AttractionParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttractionParticle, DeployLocation);
    CollisionComponent->SetSimulatePhysics(false);
    ProjectileMovementComponent->StopMovementImmediately();
    if(Force) Force->Activate();
    
    FTimerHandle Handle;
    FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AP_AttractionArea::StopArea);
    GetWorldTimerManager().SetTimer(Handle, TimerDelegate, AreaDuration, false);
}

void AP_AttractionArea::StopArea(){
    AttractionParticleComponent->Deactivate();
    AttractionParticleComponent->DestroyComponent();
    Destroy();
}
