// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetDialog.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "System/DigitalBleedGameInstance.h"

class UDigitalBleedGameInstance;

void UWidgetDialog::NativeConstruct()
{
	Super::NativeConstruct();
	this->Btn_Yes->OnClicked.AddDynamic(this, &UWidgetDialog::OnBtnYesClicked);
}

void UWidgetDialog::OnBtnYesClicked()
{
	UDigitalBleedGameInstance* GameInstance = Cast<UDigitalBleedGameInstance>(GetGameInstance());
	if (!this->DialogData.Action.IsEmpty())
	{
		
		if (GameInstance)
		{
			GameInstance->OnGlobalEvent.Broadcast(DialogData.Action);
			GameInstance->DoGlobalEvent(DialogData.Action);
		}
	}
	if (!this->DialogData.NextRowName.IsNone())
	{
		GameInstance->HideDialog();
		GameInstance->ShowDialog(GameInstance->FindDialogByRowName(this->DialogData.NextRowName));
	} else
	{
		GameInstance->HideDialog();
	}
}

void UWidgetDialog::SetDialogData(const FRowDialog& NewDialogData)
{
	UDigitalBleedGameInstance* GameInstance = Cast<UDigitalBleedGameInstance>(GetGameInstance());
	this->DialogData = NewDialogData;
	this->Text_Name->SetText(this->DialogData.Name);
	this->Text_Body->SetText(DialogData.Body);
	if (DialogData.Thumbnail->IsValidLowLevel()) this->Image_Thumbnail->SetBrushFromTexture(DialogData.Thumbnail);
	GameInstance->PlayDialogSound(DialogData.DialogSound);

	if (DialogData.NextSelectionName.IsNone())
	{
		this->Btn_Yes->SetVisibility(ESlateVisibility::Visible);
	}else
	{
		this->Btn_Yes->SetVisibility(ESlateVisibility::Hidden);
		const FRowSelection FoundSelection = GameInstance->FindDialogSelectionByRowName(this->DialogData.NextSelectionName);
		GameInstance->ShowDialogSelection(FoundSelection);
	}
}

FRowDialog& UWidgetDialog::GetDialogData()
{
	return this->DialogData;
}
