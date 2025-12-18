// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/StructDialog.h"
#include "WidgetDialog.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetDialog : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	FRowDialog DialogData;

	UFUNCTION()
	void OnBtnYesClicked();

public:
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UTextBlock* Text_Name;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class URichTextBlock* Text_Body;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UButton* Btn_Yes;

	UFUNCTION()
	void SetDialogData(const FRowDialog& NewDialogData);

	UFUNCTION()
	FRowDialog& GetDialogData();
};
