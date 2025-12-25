// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetDayTransition.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetDayTransition : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	
	

protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UTextBlock* Text_Now;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	class UTextBlock* Text_Next;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation", meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimInit;

public:
	UFUNCTION(BlueprintCallable)
	void PlayTransitionAnim();
};
