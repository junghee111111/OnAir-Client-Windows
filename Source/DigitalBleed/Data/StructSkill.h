// Copyright 2025, Ducksoo Games. All RIghts Reserved.
// 2026.01.14, Written By Junghee Wang

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StructSkill.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRowSkill : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Elemental;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinHitCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHitCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Point;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAllAttack;
};

USTRUCT(BlueprintType)
struct FRowSkillRecord : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InvestedPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Owner;
};



UCLASS()
class DIGITALBLEED_API UStructSkill : public UObject
{
	GENERATED_BODY()
};
