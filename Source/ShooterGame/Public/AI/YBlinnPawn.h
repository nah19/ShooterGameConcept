// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "ConceptCharacter.h"
#include "MinionController.h"
#include "ConceptDamageBehavior.h"

#include "GameFramework/DefaultPawn.h"
#include "YBlinnPawn.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AYBlinnPawn : public ADefaultPawn
{
	GENERATED_BODY()
	
    
public:
    bool bIsDead = false;
    
    
    AYBlinnPawn(const FObjectInitializer& ObjectInitializer);
    
    UPROPERTY(editanywhere, blueprintreadwrite, Category = "Minion")
    UParticleSystem* ExplosionClass;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minion", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SkeletalMesh;    
    
    UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "Minion")
    float Health;
    UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "Minion")
    float MaxHealt;
    UPROPERTY(replicated, visibleanywhere, blueprintreadonly, Category = "Minion")
    float Armor;
    UPROPERTY(replicated, editanywhere, blueprintreadwrite, Category = "Minion")
    int32 TeamNumber;
    UPROPERTY(replicated, editanywhere, blueprintreadwrite, Category = "Minion")
    AActor* Target_C;
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
    UFUNCTION(BlueprintCallable, Category = "Game")
    void OnMinionDestroyed();
    UFUNCTION(BlueprintCallable, Category = "Minion")
    bool IsPointAtRightOfLine(FVector PointA, FVector PointB, FVector PointToCheck);
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    
protected:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion")
    TSubclassOf<class AActor> AltenumClass;
    UPROPERTY()
    AActor* Altenum;

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
    UFUNCTION()
    void SpawnAltenumCollectable();
    
    
private:
    float currentDeltaTime;
    float StuckTime;
	
	
};
