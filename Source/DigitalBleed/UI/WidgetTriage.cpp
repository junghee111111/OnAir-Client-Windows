// Copyright 2025, Ducksoo Games. All RIghts Reserved.
// 2026-01-11, Written By Junghee Wang

#include "WidgetTriage.h"

void UWidgetTriage::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->SetShowMouseCursor(true);
	}
}

void UWidgetTriage::Close()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
	this->RemoveFromParent();
}
