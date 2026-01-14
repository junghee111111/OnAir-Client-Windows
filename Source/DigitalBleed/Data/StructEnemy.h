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
};

UCLASS()
class DIGITALBLEED_API UStructEnemy : public UObject
{
	GENERATED_BODY()
};
