// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "StaticFunctionLibrary.generated.h"

/**
 * 
 */



UENUM(BlueprintType)
enum class ETypeOfEffectEnum : uint8 {
	RE_BIOLOGICAL 	UMETA(DisplayName = "BIOLOGICAL"),
	RE_IMPACT 	UMETA(DisplayName = "IMPACT"),
	RE_ELECTRONIC	UMETA(DisplayName = "ELECTRONIC"),
	RE_ABRASIVE UMETA(DisplayName = "ABRASIVE"),
	RE_MENTAL UMETA(DisplayName = "MENTAL"),
	RE_SPACETIME UMETA(DisplayName = "SPACETIME"),
	RE_ELEMENT UMETA(DisplayName = "ELEMENT"),
};

UENUM(BlueprintType)
enum class EStateOfAbility : uint8 {
	RE_READY 	UMETA(DisplayName = "RE_READY"),
	RE_INUSE 	UMETA(DisplayName = "RE_INUSE"),
	RE_RELOADING	UMETA(DisplayName = "RE_RELOADING"),
	RE_RECOVERING UMETA(DisplayName = "RE_RECOVERING"),

};


UENUM(BlueprintType)
enum class ETypeOfAbilityEnum : uint8 {
	RE_WEAPON 	UMETA(DisplayName = "RE_WEAPON"),
	RE_OTHER 	UMETA(DisplayName = "RE_OTHER"),
};


UENUM(BlueprintType)
enum class ETypeOfFirePointEnum : uint8 {
	RE_SOCKET 	UMETA(DisplayName = "RE_SOCKET"),
	RE_RELATIVE 	UMETA(DisplayName = "RE_RELATIVE"),
};

USTRUCT(BlueprintType)
struct FAbilityData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FText NameAbility;
	UPROPERTY()
		FText DescriptionAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		UTexture2D* IconOfAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		ETypeOfAbilityEnum TypeOfAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		ETypeOfEffectEnum TypeOfEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		UTexture2D* LobbySlotImage;
    
    UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "Ability")
        float EffectDuration;

	FAbilityData()
	{
		NameAbility = FText::AsCultureInvariant("NombrePorDefecto");// NSLOCTEXT("StaticFunction", "Ability_Name", "NombrePorDefecto");
		DescriptionAbility = FText::AsCultureInvariant("Descripcion por defecto");  //NSLOCTEXT("StaticFunction", "Ability_Description", "Descripcion por defecto");

		IconOfAbility = NULL;

		TypeOfAbility = ETypeOfAbilityEnum::RE_OTHER;
		TypeOfEffect = ETypeOfEffectEnum::RE_ELECTRONIC;
        EffectDuration = 0.f;
	}

};

UENUM(BlueprintType)
namespace ECharacterEnum {
	enum CharacterSelected {
		RE_UNREAL 	UMETA(DisplayName = "RE_UNREAL"),
		RE_ARCHER 	UMETA(DisplayName = "RE_ARCHER"),
		RE_JACOB 	UMETA(DisplayName = "RE_JACOB"),
		RE_OLIVIA 	UMETA(DisplayName = "RE_OLIVIA"),
		RE_GRYMBU 	UMETA(DisplayName = "RE_GRYMBU"),
		RE_SIDI		UMETA(DisplayName = "RE_SIDI"),
		RE_CERBERUS	UMETA(DisplayName = "RE_CERBERUS"),
		RE_MULOK	UMETA(DisplayName = "RE_MULOK"),
		RE_ALEXUR	UMETA(DisplayName = "RE_ALEXUR"),
		RE_V2		UMETA(DisplayName = "RE_V2")
	};
}

UENUM(BlueprintType)
namespace EMinionEnum {
	enum MinionSelected {
		RE_SPEEDER 	UMETA(DisplayName = "RE_SPEEDER")
	};
}

USTRUCT(BlueprintType)
struct FGenericCharacterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float Life;
		UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float LifeMax;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float LifeReg;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float Shield;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float ShieldMax;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float ShieldReg;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float Armor;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float SpeedMax;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "ConceptCharacter")
		float SprintSpeed;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float JumpPower;
	UPROPERTY(blueprintreadwrite, EditInstanceOnly, Category = "ConceptCharacter")
		float Stamina;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float StaminaMax;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float StaminaReg;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float StaminaExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FGenericCharacterData")
		TEnumAsByte<ECharacterEnum::CharacterSelected> CharacterSelected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FGenericCharacterData")
		UTexture2D* CharacterImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConceptCharacter")
		USkeletalMesh* SkeletalMesh;
};

USTRUCT(BlueprintType)
struct FGenericMinionData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float LifeMax;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float LifeReg;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float ShieldMax;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float ShieldReg;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float Armor;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float SpeedMax;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FGenericCharacterData")
		float JumpPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FGenericCharacterData")
		TEnumAsByte<EMinionEnum::MinionSelected> MinionSelected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FGenericCharacterData")
		UTexture2D* IconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConceptCharacter")
		USkeletalMesh* SkeletalMesh;
};


USTRUCT(BlueprintType)
struct FKillData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FKillData")
		FString KillerGameTag;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FKillData")
		FString KillerCharacter;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FKillData")
		FString KillerNameAbility;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FKillData")
		FString VictimGameTag;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FKillData")
		FString VictimCharacter;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FKillData")
		int32 KillerTeam;
	UPROPERTY(VisibleInstanceOnly, blueprintreadonly, Category = "FKillData")
		int32 VictimTeam;

};


UCLASS(Blueprintable)
class UStaticFunctionLibrary : public UObject
{
	GENERATED_BODY()



		//FORCEINLNE function
		static FORCEINLINE bool IsValid(AActor* TheActor)
	{
		if (!TheActor) return false;
		if (!TheActor->IsValidLowLevel()) return false;
		return true;
	}

	public:
		UStaticFunctionLibrary();
	//cpp function
	static int32 ComplicatedGameDataAnalysis();

	UFUNCTION(BlueprintCallable, Category = "StaticLibrary")
		static void GetCharacterDataFromSelected(UPARAM(ref) FGenericCharacterData& Data, TEnumAsByte<ECharacterEnum::CharacterSelected> Character);
	UFUNCTION(BlueprintCallable, Category = "StaticLibrary")
		static void GetMinionDataFromSelected(UPARAM(ref) FGenericMinionData& Data, TEnumAsByte<EMinionEnum::MinionSelected> Character);
	UFUNCTION(BlueprintCallable, Category = "StaticLibrary")
		static bool IsPS4();

	static TArray<float> SortArray(const TArray<float>& MyArray, int32 top, int32 bottom);
	UFUNCTION(BlueprintCallable, Category = "StaticLibrary")
		static void QuickSort(UPARAM(ref) TArray<float>& myArray, int32 top, int32 bottom);
		static int Partition(TArray<float>& myArray, int32 top, int32 bottom);
		static bool CapsuleTraceMulti(UObject* WorldContextObject, const FVector_NetQuantize10 Start, const FVector_NetQuantize10 End, float Radius, float HalfHeight, ECollisionChannel TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, TArray<FHitResult>& OutHits, bool bIgnoreSelf);
		//static bool LineTraceMulti_NEW(UObject* WorldContextObject, const FVector Start, const FVector End, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, bool bIgnoreSelf);
		static bool LineTraceMulti(UObject* WorldContextObject, const FVector Start, const FVector End, ECollisionChannel TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, TArray<FHitResult>& OutHits, bool bIgnoreSelf);
		static void PrintString(UObject* WorldContextObject, const FString& InString, bool bPrintToScreen = true, bool bPrintToLog = true, FLinearColor TextColor = FLinearColor::Red, float Duration = 5.0f);
		static FRotator FindLookAtRotation(const FVector& Start, const FVector& Target);
		static FRotator MakeRotFromX(const FVector& X);


		template <typename ObjClass>
		static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
		{
			if (Path == NAME_None) return NULL;

			return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
		}

		//template <typename T>
		//static T* GetSpecificComponent(TArray<T*> &Components, FString ComponentName, AActor* ActorContext);

		//template <typename T>
		static UActorComponent* GetSpecificComponent(TArray<UActorComponent*> &Components, FString ComponentName, AActor* ActorContext);
};




