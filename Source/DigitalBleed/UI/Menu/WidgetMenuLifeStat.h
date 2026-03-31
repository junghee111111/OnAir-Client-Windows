// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMenuLifeStat.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetMenuLifeStat : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PlayAnimInit();
	void PlayAnimInitReverse();
	void PlayAnimStatsUp();
	void PlayAnimAcademicUp();
	void PlayAnimBraveUp();
	void PlayAnimCharmUp();
	void PlayAnimStatsUpReverse();

protected:
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	class UWidgetAnimation* AnimInit;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	class UWidgetAnimation* AnimStatsUp;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	class UWidgetAnimation* AnimAcademicUp;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	class UWidgetAnimation* AnimCharmUp;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	class UWidgetAnimation* AnimBraveUp;
};
