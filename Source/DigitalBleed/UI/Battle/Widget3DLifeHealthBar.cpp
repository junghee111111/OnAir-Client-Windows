// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "Widget3DLifeHealthBar.h"

void UWidget3DLifeHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget3DLifeHealthBar::SetTargetLife(ALife* NewTargetLife, int32 NewDisplayHp)
{
	this->TargetLife = NewTargetLife;
	this->DisplayHp = NewDisplayHp;
}

void UWidget3DLifeHealthBar::SetTargetHp(int32 NewTargetHp)
{
	this->TargetHp = NewTargetHp;
}
