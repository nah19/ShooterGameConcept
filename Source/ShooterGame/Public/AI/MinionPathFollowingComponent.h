// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Navigation/PathFollowingComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "MinionPathFollowingComponent.generated.h"


UCLASS()
class SHOOTERGAME_API UMinionPathFollowingComponent : public UCrowdFollowingComponent
{
	GENERATED_BODY()
	
	
protected:
	/** cached UCharacterMovementComponent */
	UPROPERTY(transient)
		UCharacterMovementComponent* CharacterMoveComp;

	//bool IsNextSegmentUpper();

public:
	//// used to detect properties of a path's segment a character is about to follow
	virtual void SetMoveSegment(int32 SegmentStartIndex) override;

	//// used to cache UCharacterMovementComponent we're using in SetMoveSegment implementation
	virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;

	/** follow current path segment */
	virtual void FollowPathSegment(float DeltaTime) override;

	///** check state of path following, update move segment if needed */
	virtual void UpdatePathSegment() override;

	//Nav Data Main
	//FORCEINLINE const ANavigationData* GetMainNavData(FNavigationSystem::ECreateIfEmpty CreateNewIfNoneFound)
	//{
	//	UNavigationSystem* NavSys = GetWorld()->GetNavigationSystem();
	//	if (!NavSys) return NULL;
	//	return NavSys->GetMainNavData(CreateNewIfNoneFound);
	//}

	//FORCEINLINE const ANavigationData* JoyGetNavData() const
	//{
	//	UNavigationSystem* NavSys = GetWorld()->GetNavigationSystem();

	//	const FNavAgentProperties& AgentProperties = MovementComp->GetNavAgentPropertiesRef();
	//	if (NavSys){
	//		const ANavigationData* NavData = NavSys->GetNavDataForProps(AgentProperties);
	//		if (NavData == NULL)
	//		{
	//			//VSCREENMSG("ERROR USING MAIN NAV DATA");
	//			NavData = NavSys->GetMainNavData();
	//		}
	//		return NavData;
	//	} 
	//	
	//	return NULL;
	//	
	//}

	////VERY IMPORTANT FOR CRASH PROTECTION !!!!!
	//FORCEINLINE bool TileIsValid(const ARecastNavMesh* NavMesh, int32 TileIndex) const
	//{
	//	if (!NavMesh) return false;
	//	//~~~~~~~~~~~~~~
	//	//const FBox TileBounds = NavMesh->GetNavMeshTileBounds(TileIndex);

	//	return TileBounds.IsValid != 0;
	//}

	////add this to your custom path follow component!
	//bool NavPoly_GetAllPolys(TArray<FNavPoly>& Polys);

	// Rama UE4 C++ AI Code for you!
	//     add this to your custom path follow component!

	//Nav Data Main
	//FORCEINLINE const ANavigationData* GetMainNavData(FNavigationSystem::ECreateIfEmpty CreateNewIfNoneFound)
	//{
	//	UNavigationSystem* NavSys = GetWorld()->GetNavigationSystem();
	//	if (!NavSys) return NULL;
	//	return NavSys->GetMainNavData(CreateNewIfNoneFound);
	//}

	//Choose Which Nav Data To Use
	//FORCEINLINE const ANavigationData* JoyGetNavData() const
	//{
	//	if (!MovementComp)
	//	{
	//		return GetMainNavData();
	//	}

	//	const FNavAgentProperties& AgentProperties = MovementComp->GetNavAgentPropertiesRef();
	//	const ANavigationData* NavData = GetNavDataForProps(AgentProperties);
	//	if (NavData == NULL)
	//	{
	//		//VSCREENMSG("ERROR USING MAIN NAV DATA");
	//		NavData = GetMainNavData();
	//	}

	//	return NavData;
	//}


};
