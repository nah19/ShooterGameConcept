// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "PS_Survival.h"
#include "GM_Survival.h"
#include "DB_YBlinn.h"
#include "YBlinnPawn.h"




AYBlinnPawn::AYBlinnPawn(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer) {
    PrimaryActorTick.bCanEverTick = true;

	DifficultPoints = 5;
    MaxHealt = 100;
    Health = MaxHealt;
  
    
    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

    if (SkeletalMesh)
    {
        SkeletalMesh->AlwaysLoadOnClient = true;
        SkeletalMesh->AlwaysLoadOnServer = true;
        SkeletalMesh->bOwnerNoSee = true;
        SkeletalMesh->bCastDynamicShadow = true;
        SkeletalMesh->bAffectDynamicIndirectLighting = false;
        SkeletalMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        SkeletalMesh->SetupAttachment(RootComponent);
        SkeletalMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
        SkeletalMesh->bGenerateOverlapEvents = false;
        SkeletalMesh->SetCanEverAffectNavigation(false);
    }
}

bool AYBlinnPawn::IsPointAtRightOfLine(FVector PointA, FVector PointB, FVector PointToCheck) {
    return (((PointB.Y - PointA.Y) * (PointToCheck.Z - PointA.Z)) - ((PointToCheck.Y - PointA.Y) * (PointB.Z - PointA.Z)) < 0);
}

void AYBlinnPawn::FillMinionVariablesWithData(UPARAM(ref) FGenericMinionData& DataParam) {
    
    Health = DataParam.LifeMax;
    MaxHealt = DataParam.LifeMax;
    Armor = DataParam.Armor;
}


void AYBlinnPawn::BeginPlay() {
    Super::BeginPlay();
	UStaticFunctionLibrary::GetMinionDataFromSelected(MinionData, MinionData.MinionSelected);
	FillMinionVariablesWithData(MinionData);
    MyDamageBehavior = NewObject<UDB_YBlinn>();
    MyDamageBehavior->Initialize((AActor*)this);
    CanReceiveDamage = true;
}


void AYBlinnPawn::Tick( float DeltaTime ) {
    Super::Tick( DeltaTime );
    
    currentDeltaTime = DeltaTime;
    
    
    if(bIsDead){
        FVector Force = GetCollisionComponent()->GetUpVector()*(-980.f);
        SkeletalMesh->AddForceToAllBodiesBelow(Force, "root", true, true);
    }
    
}

float AYBlinnPawn::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
    if (Health <= 0 || !CanReceiveDamage) { return 0.0f; }
    
    float fDamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    AConceptPlayerController* Controller = Cast<AConceptPlayerController>(EventInstigator);
    if (Controller){
        AConceptCharacter* Character = Cast<AConceptCharacter>(Controller->GetPawn());
        if(Character){
            Character->OtherHasReceivedDamage();
            
            if (HasAuthority()){
                TSubclassOf<UDamageType> ClaseSinCastear = DamageEvent.DamageTypeClass;
                class UConceptDamageType* ClaseCasteado = Cast<UConceptDamageType>(ClaseSinCastear.GetDefaultObject());
                
                FAbilityData Data;
                AProjectile* Projectile = Cast<AProjectile>(DamageCauser);
                AAbility* Ability = Cast<AAbility>(DamageCauser);
                if (Projectile) {
                    Data = Projectile->AbilityData;
                }
                else if (Ability) {
                    Data = Ability->AbilityData;
                }
                ClaseCasteado->HandleDamage(DamageAmount, EventInstigator, GetController(), MyDamageBehavior, (AConceptGameMode*)GetWorld()->GetAuthGameMode(), Data);
            }
        }
    }

    return DamageAmount;
}

void AYBlinnPawn::OnHover(){
    AConceptCharacter* CurrentCharacter = Cast<AConceptCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
    if (CurrentCharacter){
        CurrentCharacter->SetTarget((AActor*)this);
    }
//    GetMesh()->SetRenderCustomDepth(true);
}

void AYBlinnPawn::OnExitHover(){
    AConceptCharacter* CurrentCharacter = Cast<AConceptCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
    if (CurrentCharacter){
        CurrentCharacter->SetTarget(NULL);
        CurrentCharacter->SelectedTarget = NULL;
    }
//    GetMesh()->SetRenderCustomDepth(false);
}

void AYBlinnPawn::Death_Implementation() {

    FTimerHandle Handle;
    FTimerDelegate delegate = FTimerDelegate::CreateUObject(this, &AYBlinnPawn::DissolveMaterialEffect);
    GetWorldTimerManager().SetTimer(Handle, delegate, 5.0f, false);
    DissolveMaterialEffect();
    
    
    EnablePhysics();
    GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    AMinionController* MinionController = Cast<AMinionController>(GetController());
    if(MinionController){
        MinionController->KillingMinion();
    }
    
    
    //Destroy();
    
}

void AYBlinnPawn::EnablePhysics() {
    bIsDead = true;
    GetMovementComponent()->SetComponentTickEnabled(false);
    SkeletalMesh->SetAllBodiesBelowSimulatePhysics("root", true);
}

void AYBlinnPawn::DissolveMaterialEffect_Implementation() {
    //IMPLEMENTED IN BLUEPRINT
}

void AYBlinnPawn::OnMinionDestroyed() {
    SpawnAltenumCollectable();
    
    if (GetWorld()) {
        AGM_Survival* LocalGameMode = Cast<AGM_Survival>(GetWorld()->GetAuthGameMode());
        if (LocalGameMode) {
            
            LocalGameMode->Enemies.Remove(this);
            if (UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
                APS_Survival* LocalPlayerState = Cast<APS_Survival>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
                if (LocalPlayerState) {
                    if (LocalPlayerState->RoundEnemiesKilledClasses.Contains(this->GetClass())) {
                        int32 Value = *LocalPlayerState->RoundEnemiesKilledClasses.Find(this->GetClass());
                        LocalPlayerState->RoundEnemiesKilledClasses.Add(this->GetClass(), Value + 1);
                    }
                    else {
                        LocalPlayerState->RoundEnemiesKilledClasses.Add(this->GetClass(), 1);
                    }
                    
                    if (LocalPlayerState->TotalEnemiesKilledClasses.Contains(this->GetClass())) {
                        int32 Value = *LocalPlayerState->TotalEnemiesKilledClasses.Find(this->GetClass());
                        LocalPlayerState->TotalEnemiesKilledClasses.Add(this->GetClass(), Value + 1);
                    }
                    else {
                        LocalPlayerState->TotalEnemiesKilledClasses.Add(this->GetClass(), 1);
                    }
                }
            }
            LocalGameMode->ContinueSpawnEnemies();
        }
    }
}

void AYBlinnPawn::SpawnAltenumCollectable() {
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.Instigator = this;
    SpawnParams.bNoFail = true;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Altenum = GetWorld()->SpawnActor<AActor>(AltenumClass, GetActorLocation(), GetActorRotation(), SpawnParams);
    int32 Difficult = (int32)(DifficultPoints*2);
    UFunction* function = Altenum->FindFunction("SetAltenumQuantity");
    Altenum->ProcessEvent(function, &Difficult);
}

void AYBlinnPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AYBlinnPawn, Health);
    DOREPLIFETIME(AYBlinnPawn, TeamNumber);
    DOREPLIFETIME(AYBlinnPawn, Target_C);
}


