// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "GridNode.h"


GridNode::GridNode(FVector p, int r , int c)
{
	row = r;
	column = c;
	GlobalPosition = p;
}
GridNode::GridNode()
{
}
FString GridNode::ToString()
{
	FString s = FString::Printf(TEXT("[%d,%d]"),row,column);
	return s;
}
GridNode::~GridNode()
{
}

