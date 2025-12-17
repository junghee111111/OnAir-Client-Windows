// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetLoadingScreen.h"

void UWidgetLoadingScreen::Hide()
{
	this->PlayAnimation(this->AnimHide);
}

void UWidgetLoadingScreen::Show()
{
	this->PlayAnimation(this->AnimShow);
}
