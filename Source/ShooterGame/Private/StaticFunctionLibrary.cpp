// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Classes/Engine/Console.h"
#include "StaticFunctionLibrary.h"


UStaticFunctionLibrary::UStaticFunctionLibrary(){


}

int32 UStaticFunctionLibrary::ComplicatedGameDataAnalysis()
{
	//Do lots of stuff that requires the entire Game's compiled header source,
	//  involving lots of your custom project-specific classes,
	//    classes that have not been compiled in the .h of this library 
	//    since they depend on it.
	return 9000;
}

bool UStaticFunctionLibrary::IsPS4() {
#if PLATFORM_PS4
	return true;
#endif
	return false;
}

void UStaticFunctionLibrary::GetCharacterDataFromSelected(FGenericCharacterData& Data, TEnumAsByte<ECharacterEnum::CharacterSelected> Character)
 {


	switch (Character){
	case ECharacterEnum::RE_ARCHER:
		Data.LifeMax = 55;
		Data.LifeReg = 1;
		Data.ShieldMax = 50;
		Data.ShieldReg =20;
		Data.Armor = 10;
		Data.SpeedMax = 925;
		Data.JumpPower = 3300;
		Data.StaminaMax = 150;
		Data.StaminaReg = 1.25;
		Data.StaminaExp = 0.4f;
		Data.CharacterImage = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/Archer/archerRigg2.archerRigg2");
		break;

	case ECharacterEnum::RE_GRYMBU:
		Data.LifeMax = 75;
		Data.LifeReg = 0.5;
		Data.ShieldMax = 125;
		Data.ShieldReg = 8;
		Data.Armor = 30;
		Data.SpeedMax = 1012;
		Data.JumpPower = 2600;
		Data.StaminaMax = 125;
		Data.StaminaReg = 1.25;
		Data.StaminaExp = 0.4f;
		Data.CharacterImage = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/Grymbu/GrymbuRigged.GrymbuRigged");
		break;

	case ECharacterEnum::RE_JACOB:
		Data.LifeMax = 60;
		Data.LifeReg = 1;
		Data.ShieldMax = 75;
		Data.ShieldReg = 40;
		Data.Armor = 30;
		Data.SpeedMax = 1500;
		Data.JumpPower = 2900;
		Data.StaminaMax = 125;
		Data.StaminaReg = 1.25;
		Data.StaminaExp = 0.4f;
		Data.CharacterImage = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/NewJacob/JacobRigged.JacobRigged");
		break;

	case ECharacterEnum::RE_UNREAL:
		Data.LifeMax = 60;
		Data.LifeReg = 1;
		Data.ShieldMax = 75;
		Data.ShieldReg = 40;
		Data.Armor = 25;
		Data.SpeedMax = 1225;
		Data.JumpPower = 3100;
		Data.StaminaMax = 150;
		Data.StaminaReg = 1.25;
		Data.StaminaExp = 0.4f;
		Data.CharacterImage = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/Mannequin/SK_Mannequin.SK_Mannequin");
		break;

	case ECharacterEnum::RE_OLIVIA:
		Data.LifeMax = 80;
		Data.LifeReg = 2;
		Data.ShieldMax = 300;
		Data.ShieldReg = 20;
		Data.Armor = 30;
		Data.SpeedMax = 1600;
		Data.JumpPower = 4000;
		Data.StaminaMax = 175;
		Data.StaminaReg = 1.25;
		Data.StaminaExp = 0.4f;
		Data.CharacterImage = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/Archer/archerRigg2.archerRigg2");
		break;

	case ECharacterEnum::RE_SIDI:
		Data.LifeMax = 75;
		Data.LifeReg = 1;
		Data.ShieldMax = 40;
		Data.ShieldReg = 20;
		Data.Armor = 30;
		Data.SpeedMax = 1500;
		Data.JumpPower = 2900;
		Data.StaminaMax = 175;
		Data.StaminaReg = 1.25;
		Data.StaminaExp = 0.4f;
		Data.CharacterImage = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/Robot/RobotTest.RobotTest");
		break;

	case ECharacterEnum::RE_V2:
		Data.LifeMax = 75;
		Data.LifeReg = 1;
		Data.ShieldMax = 80;
		Data.ShieldReg = 20;
		Data.Armor = 30;
		Data.SpeedMax = 1125;
		Data.JumpPower = 2900;
		Data.StaminaMax = 125;
		Data.StaminaReg = 1.25;
		Data.StaminaExp = 0.4f;
		Data.CharacterImage = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/V2_Lobby-01.V2_Lobby-01");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/V2/v2rigg.v2rigg");
		break;

	case ECharacterEnum::RE_CERBERUS:
		Data.LifeMax = 75;
		Data.LifeReg = 0.5;
		Data.ShieldMax = 75;
		Data.ShieldReg = 20;
		Data.Armor = 30;
		Data.SpeedMax = 1150;
		Data.JumpPower = 2900;
		Data.StaminaMax = 175;
		Data.StaminaReg = 1.25;
		Data.StaminaExp = 0.4f;
		Data.CharacterImage = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/Cerberus/CerverusOptimal.CerverusOptimal");
		break;

	case ECharacterEnum::RE_MULOK:
		Data.LifeMax = 100;
		Data.LifeReg = 0.5;
		Data.ShieldMax = 75;
		Data.ShieldReg = 40;
		Data.Armor = 10;
		Data.SpeedMax = 900;
		Data.JumpPower = 2100;
		Data.StaminaMax = 100;
		Data.StaminaReg = 1.25;
		Data.StaminaExp = 0.4f;
		Data.CharacterImage = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/Mulok/Mulok.Mulok");
		break;

	default:
		Data.LifeMax = 100;
		Data.LifeReg = 0.5;
		Data.ShieldMax = 75;
		Data.ShieldReg = 20;
		Data.Armor = 30;
		Data.SpeedMax = 1150;
		Data.JumpPower = 2900;
		Data.StaminaMax = 125;
		Data.StaminaReg = 1.25;
		Data.StaminaExp = 0.4f;
		Data.CharacterImage = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/Archer/archerRigg2.archerRigg2");
		break;

	}


	Data.Life = Data.LifeMax;
	Data.Shield = Data.ShieldMax;
	Data.SprintSpeed = Data.SpeedMax * 2;
	Data.Stamina = Data.StaminaMax;
	Data.CharacterSelected = Character;
}

void UStaticFunctionLibrary::GetMinionDataFromSelected(FGenericMinionData& Data, TEnumAsByte<EMinionEnum::MinionSelected> Character)
{
	switch (Character) {
	case EMinionEnum::RE_SPEEDER:
		Data.LifeMax = 20;
		Data.LifeReg = 1;
		Data.ShieldMax = 50;
		Data.ShieldReg = 20;
		Data.Armor = 10;
		Data.SpeedMax = 2500;
		Data.JumpPower = 3300;
		Data.IconTexture = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/Archer/archerRigg2.archerRigg2");
		break;

	default:
		Data.LifeMax = 100;
		Data.LifeReg = 0.5;
		Data.ShieldMax = 75;
		Data.ShieldReg = 20;
		Data.Armor = 30;
		Data.SpeedMax = 1150;
		Data.JumpPower = 2900;
		Data.IconTexture = UStaticFunctionLibrary::LoadObjFromPath<UTexture2D>("/Game/Textures/Jacob_Thumbnail.Jacob_Thumbnail");
		Data.SkeletalMesh = UStaticFunctionLibrary::LoadObjFromPath<USkeletalMesh>("/Game/Characters/Archer/archerRigg2.archerRigg2");
		break;

	}
}



TArray<float> UStaticFunctionLibrary::SortArray(const TArray<float>& MyArray, int32 top, int32 bottom)
{
	TArray <float> tempArray = MyArray;
	QuickSort(tempArray, bottom, top);
	return tempArray;
}

void UStaticFunctionLibrary::QuickSort(TArray<float>& myArray, int32 top, int32 bottom)
{
	int middle;
	if (top < bottom)
	{
		middle = Partition(myArray, top, bottom);
		QuickSort(myArray, top, middle);   // sort first section
		QuickSort(myArray, middle + 1, bottom);    // sort second section
	}
	return;
}
int UStaticFunctionLibrary::Partition(TArray<float>& myArray, int32 top, int32 bottom)
{
	float x = myArray[top];
	int i = top - 1;
	int j = bottom + 1;
	float temp;
	do
	{
		do
		{
			j--;
		} while (x < myArray[j]);

		do
		{
			i++;
		} while (x> myArray[i]);

		if (i < j)
		{
			temp = myArray[i];
			myArray[i] = myArray[j];
			myArray[j] = temp;
		}
	} while (i < j);
	return j;
}



bool UStaticFunctionLibrary::CapsuleTraceMulti(UObject* WorldContextObject, const FVector_NetQuantize10 Start, const FVector_NetQuantize10 End, float Radius, float HalfHeight, ECollisionChannel TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, TArray<FHitResult>& OutHits, bool bIgnoreSelf) {
	static const FName CapsuleTraceMultiName(TEXT("CapsuleTraceMulti"));

	FCollisionQueryParams Params(CapsuleTraceMultiName, bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.bTraceAsyncScene = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	if (bIgnoreSelf)
	{
		AActor* IgnoreActor = Cast<AActor>(WorldContextObject);
		if (IgnoreActor)
		{
			Params.AddIgnoredActor(IgnoreActor);
		}
		else
		{
			// find owner
			UObject* CurrentObject = WorldContextObject;
			while (CurrentObject)
			{
				CurrentObject = CurrentObject->GetOuter();
				AActor* IgnoreActor = Cast<AActor>(CurrentObject);
				if (IgnoreActor)
				{
					Params.AddIgnoredActor(IgnoreActor);
					break;
				}
			}
		}
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	bool const bHit = World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, TraceChannel, FCollisionShape::MakeCapsule(Radius, HalfHeight), Params);

	if (true)
	{
		bool bPersistent = false;//DrawDebugType == EDrawDebugTrace::Persistent;
		float LifeTime = -1.0f;//(DrawDebugType == EDrawDebugTrace::ForDuration) ? KISMET_TRACE_DEBUG_DRAW_DURATION : 0.f;

							   //if (bHit && OutHits.Last().bBlockingHit)
							   //{
							   //	// Red up to the blocking hit, green thereafter
							   //	FVector_NetQuantize10 const BlockingHitPoint = OutHits.Last().Location;
							   //	::DrawDebugCapsule(World, Start, HalfHeight, Radius, FQuat::Identity, FColor::Red, bPersistent, LifeTime);
							   //	::DrawDebugCapsule(World, BlockingHitPoint, HalfHeight, Radius, FQuat::Identity, FColor::Red, bPersistent, LifeTime);
							   //	::DrawDebugLine(World, Start, BlockingHitPoint, FColor::Red, bPersistent, LifeTime);

							   //	::DrawDebugCapsule(World, End, HalfHeight, Radius, FQuat::Identity, FColor::Green, bPersistent, LifeTime);
							   //	::DrawDebugLine(World, BlockingHitPoint, End, FColor::Green, bPersistent, LifeTime);
							   //}
							   //else
							   //{
							   //	// no hit means all red
							   //	::DrawDebugCapsule(World, Start, HalfHeight, Radius, FQuat::Identity, FColor::Red, bPersistent, LifeTime);
							   //	::DrawDebugCapsule(World, End, HalfHeight, Radius, FQuat::Identity, FColor::Red, bPersistent, LifeTime);
							   //	::DrawDebugLine(World, Start, End, FColor::Red, bPersistent, LifeTime);
							   //}

							   // draw hits
							   //for (int32 HitIdx = 0; HitIdx < OutHits.Num(); ++HitIdx)
							   //{
							   //	FHitResult const& Hit = OutHits[HitIdx];
							   //	::DrawDebugPoint(World, Hit.ImpactPoint, 16.f, (Hit.bBlockingHit ? FColor::Red : FColor::Green), bPersistent, LifeTime);
							   //}
	}

	return bHit;
}

bool UStaticFunctionLibrary::LineTraceMulti(UObject* WorldContextObject, const FVector Start, const FVector End, ECollisionChannel TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, TArray<FHitResult>& OutHits, bool bIgnoreSelf)
{
	static const FName LineTraceMultiName(TEXT("LineTraceMulti"));

	FCollisionQueryParams Params(LineTraceMultiName, bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.bTraceAsyncScene = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	if (bIgnoreSelf)
	{
		AActor* IgnoreActor = Cast<AActor>(WorldContextObject);
		if (IgnoreActor)
		{
			Params.AddIgnoredActor(IgnoreActor);
		}
		else
		{
			// find owner
			UObject* CurrentObject = WorldContextObject;
			while (CurrentObject)
			{
				CurrentObject = CurrentObject->GetOuter();
				IgnoreActor = Cast<AActor>(CurrentObject);
				if (IgnoreActor)
				{
					Params.AddIgnoredActor(IgnoreActor);
					break;
				}
			}
		}
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	bool const bHit = World->LineTraceMultiByChannel(OutHits, Start, End, TraceChannel, Params);

	if (false)
	{
		bool bPersistent = true;
		float LifeTime = 1;

		// @fixme, draw line with thickness = 2.f?
		if (bHit && OutHits.Last().bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			FVector const BlockingHitPoint = OutHits.Last().ImpactPoint;
			::DrawDebugLine(World, Start, BlockingHitPoint, FColor::Red, bPersistent, LifeTime);
			::DrawDebugLine(World, BlockingHitPoint, End, FColor::Green, bPersistent, LifeTime);
		}
		else
		{
			// no hit means all red
			::DrawDebugLine(World, Start, End, FColor::Red, bPersistent, LifeTime);
		}

		// draw hits
		for (int32 HitIdx = 0; HitIdx<OutHits.Num(); ++HitIdx)
		{
			FHitResult const& Hit = OutHits[HitIdx];
			::DrawDebugPoint(World, Hit.ImpactPoint, 20, (Hit.bBlockingHit ? FColor::Red : FColor::Green), bPersistent, LifeTime);
		}
	}

	return bHit;
}

void UStaticFunctionLibrary::PrintString(UObject* WorldContextObject, const FString& InString, bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration )
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) // Do not Print in Shipping or Test

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, false);
	FString Prefix;
	if (World)
	{
		if (World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				Prefix = FString::Printf(TEXT("Client %d: "), GPlayInEditorID - 1);
				break;
			case NM_DedicatedServer:
			case NM_ListenServer:
				Prefix = FString::Printf(TEXT("Server: "));
				break;
			case NM_Standalone:
				break;
			}
		}
	}

	const FString FinalDisplayString = Prefix + InString;
	FString FinalLogString = FinalDisplayString;

	static const FBoolConfigValueHelper DisplayPrintStringSource(TEXT("Kismet"), TEXT("bLogPrintStringSource"), GEngineIni);
	if (DisplayPrintStringSource)
	{
		const FString SourceObjectPrefix = FString::Printf(TEXT("[%s] "), *GetNameSafe(WorldContextObject));
		FinalLogString = SourceObjectPrefix + FinalLogString;
	}

	if (bPrintToLog)
	{
		UE_LOG(LogBlueprintUserMessages, Log, TEXT("%s"), *FinalLogString);

		APlayerController* PC = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, 0) : NULL);
		ULocalPlayer* LocalPlayer = (PC ? Cast<ULocalPlayer>(PC->Player) : NULL);
		if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->ViewportConsole)
		{
			LocalPlayer->ViewportClient->ViewportConsole->OutputText(FinalDisplayString);
		}
	}
	else
	{
		UE_LOG(LogBlueprintUserMessages, Verbose, TEXT("%s"), *FinalLogString);
	}

	// Also output to the screen, if possible
	if (bPrintToScreen)
	{
		if (GAreScreenMessagesEnabled)
		{
			if (GConfig && Duration < 0)
			{
				GConfig->GetFloat(TEXT("Kismet"), TEXT("PrintStringDuration"), Duration, GEngineIni);
			}
		}
		else
		{
			UE_LOG(LogBlueprint, VeryVerbose, TEXT("Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."));
		}
	}
#endif
}


FRotator UStaticFunctionLibrary::FindLookAtRotation(const FVector& Start, const FVector& Target)
{
	return MakeRotFromX(Target - Start);
}

FRotator UStaticFunctionLibrary::MakeRotFromX(const FVector& X)
{
	return FRotationMatrix::MakeFromX(X).Rotator();
}

//template <typename T>
//T* UStaticFunctionLibrary::GetSpecificComponent(TArray<T*> &Components, FString ComponentName, AActor* ActorContext) {
//	ActorContext->GetComponents(Components);
//	for (auto Component : Components) {
//		if (Component->GetName() == ComponentName) {
//			return Component;
//		}
//	}
//	return NULL;
//}


UActorComponent* UStaticFunctionLibrary::GetSpecificComponent(TArray<UActorComponent*> &Components, FString ComponentName, AActor* ActorContext) {
	ActorContext->GetComponents(Components);
	for (auto Component : Components) {
		if (Component->GetName() == ComponentName) {
			return Component;
		}
	}
	return NULL;
}
