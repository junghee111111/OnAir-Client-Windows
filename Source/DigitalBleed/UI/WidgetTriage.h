// Copyright 2025, Ducksoo Games. All RIghts Reserved.
// 2026-01-11, Written By Junghee Wang

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetTriage.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetTriage : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void Close();
};
