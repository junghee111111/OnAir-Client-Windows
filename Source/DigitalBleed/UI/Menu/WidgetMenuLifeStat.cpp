// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetMenuLifeStat.h"

void UWidgetMenuLifeStat::PlayAnimInit()
{
	this->PlayAnimation(this->AnimInit, 0, 1);
}

void UWidgetMenuLifeStat::PlayAnimInitReverse()
{
	this->PlayAnimationReverse(this->AnimInit, 1, false);
}

void UWidgetMenuLifeStat::PlayAnimStatsUp()
{
	this->PlayAnimation(this->AnimStatsUp, 0, 1);
}

void UWidgetMenuLifeStat::PlayAnimAcademicUp()
{
	this->PlayAnimation(this->AnimAcademicUp, 0, 0);
}

void UWidgetMenuLifeStat::PlayAnimBraveUp()
{
	this->PlayAnimation(this->AnimBraveUp, 0, 0);
}

void UWidgetMenuLifeStat::PlayAnimCharmUp()
{
	this->PlayAnimation(this->AnimCharmUp, 0, 0);
}

void UWidgetMenuLifeStat::PlayAnimStatsUpReverse()
{
	this->PlayAnimationReverse(this->AnimStatsUp, 1, false);
}
