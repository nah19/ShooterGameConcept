// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "AI/Navigation/NavLinkProxy.h"
#include "CustomNavLink.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API ACustomNavLink : public ANavLinkProxy
{
	GENERATED_BODY()
public:

	ACustomNavLink(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "PointNavLinks")
		FVector LeftPoint;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "PointNavLinks")
		FVector RightPoint;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
