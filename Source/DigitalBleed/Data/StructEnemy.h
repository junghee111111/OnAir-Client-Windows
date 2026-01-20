// Copyright 2025, Ducksoo Games. All RIghts Reserved.
// 2026.01.14 Written By Junghee Wang

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StructEnemy.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRowEnemy : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Str;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Con;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Spd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Int;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Luk;
};

UCLASS()
class DIGITALBLEED_API UStructEnemy : public UObject
{
	GENERATED_BODY()
};
