// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetLoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetLoadingScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation", meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimHide;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation", meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimShow;

public:
	UFUNCTION()
	void Hide();

	UFUNCTION()
	void Show();
};
