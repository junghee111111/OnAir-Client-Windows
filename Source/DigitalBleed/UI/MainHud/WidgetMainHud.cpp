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
	this->Text_BTC->SetText(FText::FromString(FString::SanitizeFloat(GI->GetSavedPlayerState()->GetMoneyBTC())));
	this->Text_Won->SetText(FText::FromString(FString::FromInt(GI->GetSavedPlayerState()->GetMoneyWon())));
	
	this->Text_Cycle->SetText(FText::FromString(GI->GetCycleText(GI->GetHour())));

	FText TextYoil = FText::FromString("");
	if (GI->GetYoil() == 0)
	{
		TextYoil = FText::FromString(GI->GetUIString(FText::FromString("HUD_SUNDAY")));
	} else if (GI->GetYoil() == 1)
	{
		TextYoil = FText::FromString(GI->GetUIString(FText::FromString("HUD_MONDAY")));
	} else if (GI->GetYoil() == 2)
	{
		TextYoil = FText::FromString(GI->GetUIString(FText::FromString("HUD_TUESDAY")));
	} else if (GI->GetYoil() == 3)
	{
		TextYoil = FText::FromString(GI->GetUIString(FText::FromString("HUD_WEDNESDAY")));
	} else if (GI->GetYoil() == 4)
	{
		TextYoil = FText::FromString(GI->GetUIString(FText::FromString("HUD_THURSDAY")));
	} else if (GI->GetYoil() == 5)
	{
		TextYoil = FText::FromString(GI->GetUIString(FText::FromString("HUD_FRIDAY")));
	} else if (GI->GetYoil() == 6)
	{
		TextYoil = FText::FromString(GI->GetUIString(FText::FromString("HUD_SATURDAY")));
	}

	this->Text_Date->SetText(
	FText::Format(
NSLOCTEXT("Game", "DateFormat", "{0}/{1}/{2} ({3})"),
		FText::FromString(FString::FromInt(GI->GetYear())),
		FText::AsNumber(GI->GetMonth()),
		FText::AsNumber(GI->GetDay()),
		TextYoil
		)
	);
}
