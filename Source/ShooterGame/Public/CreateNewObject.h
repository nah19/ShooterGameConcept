

#pragma once

#include "GameFramework/Actor.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphSchema.h"
#include "CreateNewObject.generated.h"

UCLASS(Blueprintable)
class ACreateNewObject : public AActor
{
	GENERATED_BODY()

public:
		ACreateNewObject();
		//UFUNCTION(BlueprintPure, BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "Create Object From Blueprint", CompactNodeTitle = "Create", Keywords = "new create blueprint"), Category = Game)
		//static UObject* NewObjectFromBlueprint(UObject* WorldContextObject, TSubclassOf<UObject> UC);
};