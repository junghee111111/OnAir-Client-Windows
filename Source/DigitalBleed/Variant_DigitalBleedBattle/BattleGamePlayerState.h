// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BattleGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API ABattleGamePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ABattleGamePlayerState();
	void InitPlayerData();

protected:
	UPROPERTY(Transient)
	int32 CharacterLevel;

	UPROPERTY(Transient)
	int32 StatAttack;
	UPROPERTY(Transient)
	int32 StatDefense;
	UPROPERTY(Transient)
	int32 StatDodge;
	UPROPERTY(Transient)
	int32 StatConcentrate;
	UPROPERTY(Transient)
	int32 StatLuck;
	UPROPERTY(Transient)
	int32 StatSpeed;
	
	UPROPERTY(Transient)
	int32 MaxHb;
	UPROPERTY(Transient)
	float MaxNaIon;
	UPROPERTY(Transient)
	float MaxKIon;

	UPROPERTY(Transient)
	int32 Hb;
	UPROPERTY(Transient)
	float NaIon;
	UPROPERTY(Transient)
	float KIon;
	
};
