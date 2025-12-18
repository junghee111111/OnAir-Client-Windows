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
	this->PlayAnimation(this->AnimShow);
}

void UWidgetMainHud::PlayHideAnim()
{
	this->PlayAnimation(this->AnimHide);
}

void UWidgetMainHud::UpdateHud()
{
	UDigitalBleedGameInstance* GI = GetGameInstance<class UDigitalBleedGameInstance>();
	this->Text_BTC->SetText(FText::FromString(FString::SanitizeFloat(GI->GetSavedPlayerState()->GetMoneyBTC())));
	this->Text_Won->SetText(FText::FromString(FString::FromInt(GI->GetSavedPlayerState()->GetMoneyWon())));
	this->Text_Date->SetText(
	FText::Format(
NSLOCTEXT("Game", "DateFormat", "{0}/{1}/{2}"),
		FText::AsNumber(GI->GetYear()),
		FText::AsNumber(GI->GetMonth()),
		FText::AsNumber(GI->GetDay())
		)
	);
	if (GI->GetHour() == 0)
	{
		this->Text_Cycle->SetText(FText::FromString(GI->GetUIString(FText::FromString("HUD_MORNING"))));
	} else if (GI->GetHour() == 1)
	{
		this->Text_Cycle->SetText(FText::FromString(GI->GetUIString(FText::FromString("HUD_BRUNCH"))));
	} else if (GI->GetHour() == 2)
	{
		this->Text_Cycle->SetText(FText::FromString(GI->GetUIString(FText::FromString("HUD_LUNCH"))));
	} else if (GI->GetHour() == 3)
	{
		this->Text_Cycle->SetText(FText::FromString(GI->GetUIString(FText::FromString("HUD_EVENING"))));
	} else if (GI->GetHour() == 4)
	{
		this->Text_Cycle->SetText(FText::FromString(GI->GetUIString(FText::FromString("HUD_AFTERSCHOOL"))));
	} else if (GI->GetHour() == 5)
	{
		this->Text_Cycle->SetText(FText::FromString(GI->GetUIString(FText::FromString("HUD_DINNER"))));
	} else if (GI->GetHour() == 6)
	{
		this->Text_Cycle->SetText(FText::FromString(GI->GetUIString(FText::FromString("HUD_NIGHT"))));
	}
}
