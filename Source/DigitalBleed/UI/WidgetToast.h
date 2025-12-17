// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetToast.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetToast : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation", meta = (BindWidgetAnim))
    class UWidgetAnimation* AnimHide;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation", meta = (BindWidgetAnim))
    class UWidgetAnimation* AnimShow;
};
