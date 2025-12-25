// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetDialogSelectionButton.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetDialogSelectionButton : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

	FText ButtonText;
	FText ButtonAction;
	
protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UTextBlock* ButtonLabel;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UButton* ButtonSelection;

	UFUNCTION()
	void OnButtonClicked();

public:
	UFUNCTION()
	void SetSelectionData(const FText& NewButtonText, const FText& NewButtonAction);
};
