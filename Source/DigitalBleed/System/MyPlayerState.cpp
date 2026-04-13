// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "MyPlayerState.h"

namespace LifeStatConstants
{
	constexpr int32 IntelMaxExpTable[] = { 0, 5, 7, 9, 11 }; // 인덱스 0은 사용 안함, 레벨 1~4
	constexpr int32 MaxLevel = 4;
}


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

void AMyPlayerState::SetDayInfo(int32 NewMonth, int32 NewDay, int32 NewCycle, int32 NewYoil)
{
	this->Month = NewMonth;
	this->Day = NewDay;
	this->Cycle = NewCycle;
	this->Yoil = NewYoil;
}

bool AMyPlayerState::IncIntel()
{
	// 최대 레벨 체크
	if (this->LStat_Intel > LifeStatConstants::MaxLevel)
	{
		return false;
	}
	
	// 현재 레벨의 maxExp 가져오기
	const int32 maxExp = LifeStatConstants::IntelMaxExpTable[this->LStat_Intel];
	
	// 경험치 증가
	this->LStat_Intel_Exp++;
	
	// maxExp를 넘으면 레벨업
	if (this->LStat_Intel_Exp >= maxExp)
	{
		this->LStat_Intel++;
		this->LStat_Intel_Exp = 0; // 경험치 초기화
		return true;
	}
	
	return false;
}

bool AMyPlayerState::IncCharm()
{
	// 최대 레벨 체크
	if (this->LStat_Charm > LifeStatConstants::MaxLevel)
	{
		return false;
	}
	
	// 현재 레벨의 maxExp 가져오기
	const int32 maxExp = LifeStatConstants::IntelMaxExpTable[this->LStat_Charm];
	
	// 경험치 증가
	this->LStat_Charm_Exp++;
	
	// maxExp를 넘으면 레벨업
	if (this->LStat_Charm_Exp >= maxExp)
	{
		this->LStat_Charm++;
		this->LStat_Charm_Exp = 0; // 경험치 초기화
		return true;
	}
	
	return false;
}

bool AMyPlayerState::IncBrave()
{
	// 최대 레벨 체크
	if (this->LStat_Brave > LifeStatConstants::MaxLevel)
	{
		return false;
	}
	
	// 현재 레벨의 maxExp 가져오기
	const int32 maxExp = LifeStatConstants::IntelMaxExpTable[this->LStat_Brave];
	
	// 경험치 증가
	this->LStat_Brave_Exp++;
	
	// maxExp를 넘으면 레벨업
	if (this->LStat_Brave_Exp >= maxExp)
	{
		this->LStat_Brave++;
		this->LStat_Brave_Exp = 0; // 경험치 초기화
		return true;
	}
	
	return false;
}
