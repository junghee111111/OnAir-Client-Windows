// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetDialogSelection.h"

void UWidgetDialogSelection::SetSelectionData(const FRowSelection& NewSelectionData)
{
	this->SelectionData = NewSelectionData;
}

FRowSelection& UWidgetDialogSelection::GetSelectionData()
{
	return this->SelectionData;
}
