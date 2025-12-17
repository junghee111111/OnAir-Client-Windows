// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetModal.h"

#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "System/DigitalBleedGameInstance.h"

class UDigitalBleedGameInstance;

void UWidgetModal::NativeConstruct()
{
	Super::NativeConstruct();

	// add dynamic to Btn_Yes
	this->Btn_Yes->OnClicked.AddDynamic(this, &UWidgetModal::OnBtnYesClicked);
}

void UWidgetModal::OnBtnYesClicked()
{
	if (!ModalData.YesAction.IsEmpty())
	{
		UDigitalBleedGameInstance* GameInstance = Cast<UDigitalBleedGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			GameInstance->OnGlobalEvent.Broadcast(ModalData.YesAction);
		}
	}
}

void UWidgetModal::SetModalData(const FRowModal& NewModalData)
{
	this->ModalData = NewModalData;
	this->Text_Title->SetText(this->ModalData.Title);
	this->Text_Body->SetText(ModalData.Body);
}

FRowModal& UWidgetModal::GetModalData()
{
	return this->ModalData;
}