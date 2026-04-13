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
	// Life Stats
	UPROPERTY(BlueprintReadWrite, Category = "Life Stats")
	int32 LStat_Intel = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Life Stats")
	int32 LStat_Charm = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Life Stats")
	int32 LStat_Brave = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Life Stats")
	int32 LStat_Intel_Exp = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Life Stats")
	int32 LStat_Charm_Exp = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Life Stats")
	int32 LStat_Brave_Exp = 0;
	
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

	// ========== 어빌리티 ==========
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Ability")
	int32 Hp;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Ability")
	int32 HpMax;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Ability")
	int32 Exp;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Ability")
	int32 Level;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Ability")
	float Potassium;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Ability")
	float Sodium;

	UPROPERTY(BlueprintReadOnly, Replicated, Category="Ability")
	float Hemoglobin;

	// 돈 관련
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Money")
	int32 MoneyWon;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Money")
	float MoneyBTC;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Date")
	int32 Month;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Date")
	int32 Day;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Date")
	int32 Yoil;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Date")
	int32 Cycle;

	

public:
	// Replication 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	int32 GetMoneyWon() const { return MoneyWon; }

	UFUNCTION(BlueprintCallable)
	float GetMoneyBTC() const { return MoneyBTC; }

	UFUNCTION()
	void SetDayInfo(int32 NewMonth, int32 NewDay, int32 NewCycle, int32 NewYoil);

	UFUNCTION()
	bool IncIntel();
	bool IncCharm();
	bool IncBrave();
};
