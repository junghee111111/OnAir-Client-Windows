// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StructSelection.generated.h"

USTRUCT(BlueprintType)
struct FRowSelection : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> Actions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextRowName;
};

UCLASS()
class DIGITALBLEED_API UStructSelection : public UObject
{
	GENERATED_BODY()
};
