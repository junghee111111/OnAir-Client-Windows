// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMyPlayerState();

protected:
	// 플레이어 스탯
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	int32 Stat_STR;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	int32 Stat_CON;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	int32 Stat_DEF;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	int32 Stat_INT;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	int32 Stat_LUK;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	int32 Stat_SPD;

	// 돈 관련
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Money")
	int32 MoneyWon;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Money")
	float MoneyBTC;

public:
	// Replication 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
