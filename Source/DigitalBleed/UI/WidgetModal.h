// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/StructModal.h"
#include "WidgetModal.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetModal : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	FRowModal ModalData;

	UFUNCTION()
	void OnBtnYesClicked();
	
public:
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UTextBlock* Text_Title;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class URichTextBlock* Text_Body;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UButton* Btn_Yes;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UButton* Btn_No;

	UFUNCTION()
	void SetModalData(const FRowModal& NewModalData);

	UFUNCTION()
	FRowModal& GetModalData();
	
};
