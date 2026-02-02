// Copyright 2025, Ducksoo Games. All RIghts Reserved.
// 2026.01.22, Written By Junghee Wang
#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "UObject/Object.h"
#include "StructItem.generated.h"


USTRUCT(BlueprintType)
struct FRowItem : public FTableRowBase
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
	int32 HpInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HbInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KInc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NaInc;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ExtraEffect1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ExtraEffect2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ExtraEffect3;

	// 0 : Battle-non-usable, 1 : Both-usable, 2 : Battle-only
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CanUseInBattle;

	// 0 : Players, 1 : Enemies
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Target = 0;

	// 1 : Consume 2 : Equip 3 : Etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Type;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UNiagaraSystem* PreEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UNiagaraSystem* ProjectileEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UNiagaraSystem* HitEffect;
	
};

USTRUCT(BlueprintType)
struct FRowItemRecord : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Qty;
};



/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UStructItem : public UObject
{
	GENERATED_BODY()
};
