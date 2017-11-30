// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "AGrid.h"

#define WORLD_SCALE_MULTIPLIER 100.0f


// Sets default values
AAGrid::AAGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAGrid::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AAGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::MakeRandomColor(), FString::Printf(TEXT("NodeSize es [%d]"), NodeSize));


}

void AAGrid::BeginDestroy()
{
	Super::BeginDestroy();
	/*for (int i = 0; i < filas; i++)
	{
		delete[] NodeMatrix[i];
	}*/
}
#if  WITH_EDITOR
void AAGrid::PostEditChangeProperty(FPropertyChangedEvent & e)
{

	if (e.Property != NULL) {
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::MakeRandomColor(), TEXT("" + e.Property->GetFName().ToString()));

		FName PropertyName =  e.Property->GetFName();
		if (PropertyName == GET_MEMBER_NAME_CHECKED(AAGrid, Draw)) {
			if (GridMesh && NodeSize!=0 ) {
				cleanMemory();
				GEngine->AddOnScreenDebugMessage(-1,3.0f, FColor::Emerald, FString::Printf(TEXT("Name: %s"), *GridMesh->GetFName().ToString()));
				FVector Size = GridMesh->GetTransform().GetScale3D();
				CoordOrigin = GridMesh->GetActorLocation();
				CoordOrigin -= FVector(0, -Size.Y, Size.Z)/2 * WORLD_SCALE_MULTIPLIER;
				DrawDebugSphere(GetWorld(), CoordOrigin , NodeSize, 4, FColor(0, 255, 0), false, 5.0f, 0);
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, FString::Printf(TEXT("Extents: %s"), *Size.ToString()));
			
				filas = Size.Z/ NodeSize;
				columnas = Size.Y / NodeSize;

				NodeMatrix = new GridNode*[filas];
				for (int i = 0; i < filas; i++)
				{
					NodeMatrix[i] = new GridNode[columnas];
				}

				for (int i = 0; i < filas; i++)
				{
					FString row_s = FString("");
					for (int j = 0; j < columnas; j++)
					{
						
						NodeMatrix[i][j] = GridNode(CoordOrigin + FVector(0, -j-0.5, i+0.5)*(WORLD_SCALE_MULTIPLIER*NodeSize) , i, j);

						if(Draw)DrawDebugSphere(GetWorld(),NodeMatrix[i][j].GlobalPosition,WORLD_SCALE_MULTIPLIER*NodeSize,4,FColor(i*1.0f/filas*255.0, j*1.0f/columnas*255.0f, 0),	false,5.0f,	0);
						row_s.Append(NodeMatrix[i][j].ToString());
					}
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::MakeRandomColor(),row_s);
				}

			}
			else {
//				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::MakeRandomColor(), TEXT(" O Grid es null o el tamaño es 0, al loro!"));
			}
		}
	}

	/*for (int i = 0; i < filas; i++)
	{
		FString row_s = FString("");
		for (int j = 0; j < columnas; j++)
		{
			//NodeMatrix[i][j] = GridNode(FVector::ZeroVector, i, j);

			row_s.Append(NodeMatrix[i][j].ToString());
		}
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::MakeRandomColor(), row_s);
	}*/

	Super::PostEditChangeProperty(e);
}
void AAGrid::getPath(FVector A, FVector B)
{

}
void AAGrid::cleanMemory()
{
	for (int i = 0; i < filas; i++)
	{
	 delete[] NodeMatrix[i];
	}
	delete[] NodeMatrix;
}
#endif


