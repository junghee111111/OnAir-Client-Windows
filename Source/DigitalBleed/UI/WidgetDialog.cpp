// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetDialog.h"

#include "System/DigitalBleedGameInstance.h"

class UDigitalBleedGameInstance;

void UWidgetDialog::OnBtnYesClicked()
{
	if (!this->DialogData.Action.IsEmpty())
	{
		UDigitalBleedGameInstance* GameInstance = Cast<UDigitalBleedGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			GameInstance->OnGlobalEvent.Broadcast(DialogData.Action);
		}
	}
	if (!this->DialogData.NextRowName.IsNone())
	{
		
	}
}

void UWidgetDialog::SetDialogData(const FRowDialog& NewDialogData)
{
	this->DialogData = NewDialogData;
}

FRowDialog& UWidgetDialog::GetDialogData()
{
	return this->DialogData;
}
