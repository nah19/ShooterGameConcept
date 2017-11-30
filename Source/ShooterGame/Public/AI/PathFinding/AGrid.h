// Todos los derechos reservados por Catness Game Studios

#pragma once

#include "GameFramework/Actor.h"
#include "GridNode.h"
#include "AGrid.generated.h"

UCLASS()
class SHOOTERGAME_API AAGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAGrid();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;
	void PostEditChangeProperty(struct FPropertyChangedEvent& e);

	void getPath(FVector A, FVector B);

	UPROPERTY(EditInstanceOnly,Blueprintreadwrite,  Category = "AI")
	float NodeSize;
	UPROPERTY(EditInstanceOnly, Blueprintreadwrite, Category = "AI")
	bool Draw;
	UPROPERTY(EditInstanceOnly,blueprintreadwrite ,Category = "AI")
	AStaticMeshActor* GridMesh;
	
private:

	int32 filas;
	int32 columnas;
	GridNode** NodeMatrix;
	FVector CoordOrigin;
	void cleanMemory();
};
