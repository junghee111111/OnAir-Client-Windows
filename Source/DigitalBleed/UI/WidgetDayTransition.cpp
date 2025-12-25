// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetDayTransition.h"

#include "Components/TextBlock.h"
#include "System/DigitalBleedGameInstance.h"

void UWidgetDayTransition::NativeConstruct()
{
	Super::NativeConstruct();
	UDigitalBleedGameInstance* GI = GetGameInstance<class UDigitalBleedGameInstance>();
	
	this->Text_Now->SetText(FText::FromString(GI->GetCycleText(GI->GetHour())));
	this->Text_Next->SetText(FText::FromString(GI->GetCycleText(GI->GetHour()+1)));
}

void UWidgetDayTransition::PlayTransitionAnim()
{
	this->PlayAnimation(this->AnimInit);
}
