// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetDialogSelectionButton.h"

#include "Components/Button.h"
#include "System/DigitalBleedGameInstance.h"

void UWidgetDialogSelectionButton::NativeConstruct()
{
	Super::NativeConstruct();
	this->ButtonSelection->OnClicked.AddDynamic(this, &UWidgetDialogSelectionButton::OnButtonClicked);
}

void UWidgetDialogSelectionButton::OnButtonClicked()
{
	UDigitalBleedGameInstance* GI = GetGameInstance<UDigitalBleedGameInstance>();
	if (GI)
	{
		GI->ShowDialog(GI->FindDialogByRowName(FName(this->ButtonAction.ToString())));
	}
}

void UWidgetDialogSelectionButton::SetSelectionData(const FText& NewButtonText, const FText& NewButtonAction)
{
	this->ButtonText = NewButtonText;
	this->ButtonAction = NewButtonAction;
}
