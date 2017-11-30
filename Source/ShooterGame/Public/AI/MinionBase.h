// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameFramework/Pawn.h"
#include "MinionBase.generated.h"

UCLASS()
class SHOOTERGAME_API AMinionBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMinionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
