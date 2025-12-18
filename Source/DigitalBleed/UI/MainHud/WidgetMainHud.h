// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMainHud.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetMainHud : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void PlayShowAnim();

	UFUNCTION(BlueprintCallable)
	void PlayHideAnim();

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation", meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimHide;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation", meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimShow;

	UFUNCTION(BlueprintCallable)
	void UpdateHud();

protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UTextBlock* Text_Date;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UTextBlock* Text_Cycle;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UTextBlock* Text_Won;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UTextBlock* Text_BTC;
};
