// Copyright 2025, Ducksoo Games. All RIghts Reserved.
// 2026.02.02, Written by Junghee Wang

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StructExpTable.generated.h"

USTRUCT(BlueprintType)
struct FRowExperience : public FTableRowBase
{
	GENERATED_BODY()

	// 해당 레벨에 도달하기 위해 필요한 총 누적 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxExp = 0;
};

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UStructExpTable : public UObject
{
	GENERATED_BODY()
};
