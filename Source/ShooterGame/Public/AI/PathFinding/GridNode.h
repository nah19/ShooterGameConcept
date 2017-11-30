// Todos los derechos reservados por Catness Game Studios

#pragma once
#include <string>
/**
 * 
 */
class SHOOTERGAME_API GridNode
{
public:
	GridNode(FVector p, int r, int c);
	GridNode();
	FString ToString();
	~GridNode();
	FVector GlobalPosition;

private:
	
	int row;
	int column;
	float heuristicWeight;
};
