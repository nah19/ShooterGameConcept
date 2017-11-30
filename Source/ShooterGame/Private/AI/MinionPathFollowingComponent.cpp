// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "MinionPathFollowingComponent.h"
#include "ConceptCharacter.h"
#include "NavArea_Jump.h"
#include "AI/Navigation/NavigationSystem.h"
#include "MinionCharacter.h"

void UMinionPathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex) {
	
	if (CharacterMoveComp != NULL) {
		const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];
		const FNavPathPoint& SegmentEnd = Path->GetPathPoints()[MoveSegmentEndIndex];
		if (FNavAreaHelper::HasJumpFlag(SegmentStart)){
			AMinionCharacter* Minion = Cast<AMinionCharacter>(CharacterMoveComp->GetCharacterOwner());
			if (Minion) {
				GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Emerald, FString::Printf(TEXT("Minion Jump")));
				//Minion->JumpFromTo(SegmentStart.Location, SegmentEnd.Location);
				Minion->JumpFromToWithFixedHeight(SegmentStart.Location, SegmentEnd.Location);
			}
		}
	}
    Super::SetMoveSegment(SegmentStartIndex);
}

void UMinionPathFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp) {
	Super::SetMovementComponent(MoveComp);

	CharacterMoveComp = Cast<UCharacterMovementComponent>(MovementComp);
}

void UMinionPathFollowingComponent::UpdatePathSegment(){
	Super::UpdatePathSegment();
}

void UMinionPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	if (MovementComp == NULL || !Path.IsValid())
	{
		return;
	}
	//TArray<FNavPoly> Polys;
	//if (NavPoly_GetAllPolys(Polys)){
	//	FVector LastPoly = FVector::ZeroVector;
	//	for (int32 v = 0; v < Polys.Num(); v++)
	//	{
	//		
	//		//DrawDebugPoint(GetWorld(), Polys[v].Center, 20.0f, FColor::MakeRandomColor());
	//		if (!LastPoly.IsZero()){
	//			//DrawDebugLine(GetWorld(), LastPoly, Polys[v].Center, FColor::MakeRandomColor());
	//		}
	//		LastPoly = Polys[v].Center;
	//	}
	//}
	//if (Path->IsPartial()) //AI could not reach player, try using jump pathing!
	//{
	//	//I send out instructions to my custom character class here
	//	//JoyChar->ReceiveJumpFallPathingRequest();
	//	return;

	//}

	Super::FollowPathSegment(DeltaTime);
}



//bool UMinionPathFollowingComponent::NavPoly_GetAllPolys(TArray<FNavPoly>& Polys)
//{
//	if (!MovementComp) return false;
//	//~~~~~~~~~~~~~~~~~~
//
//	//Get Nav Data
//	
//	const ANavigationData* NavData = GetWorld()->GetNavigationSystem()->GetMainNavData();
//
//	const ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
//	if (!NavMesh)
//	{
//		return false;
//	}
//
//	TArray<FNavPoly> EachPolys;
//	for (int32 v = 0; v < NavMesh->GetNavMeshTilesCount(); v++)
//	{
//		if (!TileIsValid(NavMesh, v)) {
//			continue;
//		}
//
//		NavMesh->GetPolysInTile(v, EachPolys);
//	}
//
//	Polys = EachPolys;
//	////Add them all!
//
//	//NavMesh->DrawDebugPathCorridor(&EachPolys[0].Ref, 30);
//
//	//for (int32 v = 0; v < EachPolys.Num(); v++)
//	//{
//	//	NavMesh->DrawDebugPathCorridor(&EachPolys[v].Ref, 30);
//	//}
//	return true;
//}
