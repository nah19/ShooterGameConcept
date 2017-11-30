// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "Projectiles/Projectile.h"
#include "P_AttractionArea.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AP_AttractionArea : public AProjectile
{
	GENERATED_BODY()
	
public:
    AP_AttractionArea(const FObjectInitializer& ObjectInitializer);
    
    void Deploy();
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile")
    float DeployTime = 1.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile")
    float AreaDuration = 5.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile")
    UParticleSystem* AttractionParticle;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile")
    URadialForceComponent* Force;
	
protected:
    virtual void Tick(float DeltaSeconds) override;
    virtual void BeginPlay() override;
    void StopArea();
    
    
    bool bDeployed = false;
    FVector DeployLocation;
    UParticleSystemComponent* AttractionParticleComponent;
    
	
};
