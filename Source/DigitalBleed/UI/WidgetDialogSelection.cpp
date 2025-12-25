// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetDialogSelection.h"

#include "Components/TextBlock.h"

void UWidgetDialogSelection::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidgetDialogSelection::SetSelectionData(const FRowSelection& NewSelectionData)
{
	this->SelectionData = NewSelectionData;
	this->Text_Title->SetText(this->SelectionData.Title);
}

FRowSelection& UWidgetDialogSelection::GetSelectionData()
{
	return this->SelectionData;
}
