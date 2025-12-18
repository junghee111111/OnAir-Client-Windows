// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetLoadingScreen.h"

void UWidgetLoadingScreen::Hide()
{
	this->PlayAnimation(this->AnimHide);
}

void UWidgetLoadingScreen::Show()
{
	UE_LOG(LogTemp, Log, TEXT("[WidgetLoadingScreen] Showing Loading Screen"));
	this->PlayAnimation(this->AnimShow);
}
