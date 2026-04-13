// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerState.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameEternalHighschool.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API USaveGameEternalHighschool : public USaveGame
{
	GENERATED_BODY()

public:
	// Actor 포인터 대신 실제 데이터를 저장
	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 LStat_Intel;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 LStat_Charm;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 LStat_Brave;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 LStat_Intel_Exp;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 LStat_Charm_Exp;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 LStat_Brave_Exp;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Stat_STR;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Stat_CON;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Stat_DEF;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Stat_INT;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Stat_LUK;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Stat_SPD;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Hp;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 HpMax;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Exp;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Level;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	float Potassium;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	float Sodium;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	float Hemoglobin;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 MoneyWon;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	float MoneyBTC;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Month;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Day;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Yoil;

	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 Cycle;

	UFUNCTION()
	void CopyFrom(AMyPlayerState* Ps);

};
