// Copyright 2025, Ducksoo Games. All RIghts Reserved.
// 2026-01-11, Written By Junghee Wang

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetGlobalToast.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetGlobalToast : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

private:
	FTimerHandle HideTimerHandle;

protected:
	UPROPERTY(BlueprintReadOnly)
	class URichTextBlock* RT_Message;

	UPROPERTY(meta=(BindWidgetAnim), BlueprintReadOnly, Transient)
	class UWidgetAnimation* AnimShow;
	UPROPERTY(meta=(BindWidgetAnim), BlueprintReadOnly, Transient)
	class UWidgetAnimation* AnimHide;

public:
	UFUNCTION(BlueprintCallable)
	void Show(FString Text);

	UFUNCTION(BlueprintCallable)
	void Hide();
};
