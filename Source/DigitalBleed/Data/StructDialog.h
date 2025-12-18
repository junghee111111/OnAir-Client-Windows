// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StructDialog.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FRowDialog : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DialogSound;
};


UCLASS()
class DIGITALBLEED_API UStructDialog : public UObject
{
	GENERATED_BODY()
};
