// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StructModal.generated.h"

USTRUCT(BlueprintType)
struct FRowModal : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString YesAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NoAction;
};


UCLASS()
class DIGITALBLEED_API UStructModal : public UObject
{
	GENERATED_BODY()
};
