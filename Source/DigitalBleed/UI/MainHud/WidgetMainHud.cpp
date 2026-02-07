// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetMainHud.h"

#include "Components/TextBlock.h"
#include "System/DigitalBleedGameInstance.h"

void UWidgetMainHud::NativeConstruct()
{
	Super::NativeConstruct();
	this->UpdateHud();
	this->PlayShowAnim();
}

void UWidgetMainHud::PlayShowAnim()
{
	this->PlayAnimation(this->AnimShow, 0);
}

void UWidgetMainHud::PlayHideAnim()
{
	this->PlayAnimation(this->AnimHide, 0);
}

void UWidgetMainHud::UpdateHud()
{
	UDigitalBleedGameInstance* GI = GetGameInstance<class UDigitalBleedGameInstance>();
	this->Text_BTC->SetText(FText::FromString(FString::SanitizeFloat(GI->GetMoneyBtc())));
	this->Text_Won->SetText(FText::FromString(FString::FromInt(GI->GetMoneyWon())));
	
	this->Text_Cycle->SetText(FText::FromString(GI->GetCycleText(GI->GetHour())));
	this->Text_Date->SetText(GI->GetTodayText());
}
