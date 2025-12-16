// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "MyPlayerState.h"

AMyPlayerState::AMyPlayerState()
{
	this->MoneyBTC = 0.0f;
	this->MoneyWon = 0;
	this->Stat_CON = 4;
	this->Stat_STR = 4;
	this->Stat_DEF = 4;
	this->Stat_LUK = 4;
	this->Stat_INT = 4;
	this->Stat_SPD = 4;
	this->Hp = 100;
	this->HpMax = 100;
	this->Sodium = 140.0f;
	this->Potassium = 4.5f;
	this->Hemoglobin = 12.0f;
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
