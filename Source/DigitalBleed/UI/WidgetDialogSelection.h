// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WidgetDialogSelectionButton.h"
#include "Blueprint/UserWidget.h"
#include "Data/StructSelection.h"
#include "WidgetDialogSelection.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetDialogSelection : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	FRowSelection SelectionData;

	UPROPERTY()
	UWidgetDialogSelectionButton* WbpDialogSelectionButton = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnAir|UI")
	TSubclassOf<UWidgetDialogSelectionButton> WbpDialogSelectionButtonClass;

public:
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UTextBlock* Text_Title;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UVerticalBox* VBox_Selections;

	UFUNCTION()
	void SetSelectionData(const FRowSelection& NewSelectionData);

	UFUNCTION()
	FRowSelection& GetSelectionData();
};
