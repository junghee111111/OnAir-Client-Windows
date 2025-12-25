// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetDialogSelection.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UWidgetDialogSelection::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidgetDialogSelection::SetSelectionData(const FRowSelection& NewSelectionData)
{
	this->SelectionData = NewSelectionData;
	this->Text_Title->SetText(this->SelectionData.Title);
	this->VBox_Selections->ClearChildren();

	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	
	for (int32 i = 0; i < this->SelectionData.Actions.Num(); i++)
	{
		UWidgetDialogSelectionButton* WbpButton = CreateWidget<class UWidgetDialogSelectionButton>(PC,WbpDialogSelectionButtonClass);
		WbpButton->SetSelectionData(this->SelectionData.Body[i], this->SelectionData.Actions[i]);
		this->VBox_Selections->AddChildToVerticalBox(WbpButton);
	}
}

FRowSelection& UWidgetDialogSelection::GetSelectionData()
{
	return this->SelectionData;
}
