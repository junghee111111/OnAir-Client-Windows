// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "SaveGameEternalHighschool.h"

void USaveGameEternalHighschool::CopyFrom(AMyPlayerState* Ps)
{
	if (!Ps)
	{
		UE_LOG(LogTemp, Error, TEXT("[SaveGame] : PlayerState is null!"));
		return;
	}

	// 리플렉션을 사용하여 protected 멤버 접근
	UClass* PlayerStateClass = Ps->GetClass();

	// Life Stats
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("LStat_Intel")))
	{
		Prop->GetValue_InContainer(Ps, &LStat_Intel);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("LStat_Charm")))
	{
		Prop->GetValue_InContainer(Ps, &LStat_Charm);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("LStat_Brave")))
	{
		Prop->GetValue_InContainer(Ps, &LStat_Brave);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("LStat_Intel_Exp")))
	{
		Prop->GetValue_InContainer(Ps, &LStat_Intel_Exp);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("LStat_Charm_Exp")))
	{
		Prop->GetValue_InContainer(Ps, &LStat_Charm_Exp);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("LStat_Brave_Exp")))
	{
		Prop->GetValue_InContainer(Ps, &LStat_Brave_Exp);
	}

	// Stats
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Stat_STR")))
	{
		Prop->GetValue_InContainer(Ps, &Stat_STR);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Stat_CON")))
	{
		Prop->GetValue_InContainer(Ps, &Stat_CON);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Stat_DEF")))
	{
		Prop->GetValue_InContainer(Ps, &Stat_DEF);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Stat_INT")))
	{
		Prop->GetValue_InContainer(Ps, &Stat_INT);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Stat_LUK")))
	{
		Prop->GetValue_InContainer(Ps, &Stat_LUK);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Stat_SPD")))
	{
		Prop->GetValue_InContainer(Ps, &Stat_SPD);
	}

	// Ability
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Hp")))
	{
		Prop->GetValue_InContainer(Ps, &Hp);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("HpMax")))
	{
		Prop->GetValue_InContainer(Ps, &HpMax);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Exp")))
	{
		Prop->GetValue_InContainer(Ps, &Exp);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Level")))
	{
		Prop->GetValue_InContainer(Ps, &Level);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Potassium")))
	{
		Prop->GetValue_InContainer(Ps, &Potassium);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Sodium")))
	{
		Prop->GetValue_InContainer(Ps, &Sodium);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Hemoglobin")))
	{
		Prop->GetValue_InContainer(Ps, &Hemoglobin);
	}

	// Money (public이므로 직접 접근)
	MoneyWon = Ps->GetMoneyWon();
	MoneyBTC = Ps->GetMoneyBTC();

	// Date
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Month")))
	{
		Prop->GetValue_InContainer(Ps, &Month);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Day")))
	{
		Prop->GetValue_InContainer(Ps, &Day);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Yoil")))
	{
		Prop->GetValue_InContainer(Ps, &Yoil);
	}
	if (FProperty* Prop = PlayerStateClass->FindPropertyByName(TEXT("Cycle")))
	{
		Prop->GetValue_InContainer(Ps, &Cycle);
	}

	UE_LOG(LogTemp, Log, TEXT("[SaveGame] : Data copied from PlayerState successfully"));
}
