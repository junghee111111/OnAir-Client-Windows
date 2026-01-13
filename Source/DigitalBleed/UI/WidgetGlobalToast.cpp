// Copyright 2025, Ducksoo Games. All RIghts Reserved.
// 2026-01-11, Written By Junghee Wang

#include "WidgetGlobalToast.h"

#include "Components/RichTextBlock.h"

void UWidgetGlobalToast::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidgetGlobalToast::Show(FString Text)
{
	this->RT_Message->SetText(FText::FromString(Text));
	this->PlayAnimation(this->AnimShow);

	if (GetWorld()->GetTimerManager().IsTimerActive(HideTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
	}
	
	GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, this, &UWidgetGlobalToast::Hide, 3.0f, false);
}

void UWidgetGlobalToast::Hide()
{
	this->PlayAnimation(this->AnimHide);
}
