// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetFxLifeStatUp.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetFxLifeStatUp : public UUserWidget
{
	GENERATED_BODY()

	//Animations
	virtual void NativeConstruct() override;

public:
	void PlayAnimInit();

protected:
		UPROPERTY(Transient, meta=(BindWidgetAnim))
		class UWidgetAnimation* AnimInit;
};
