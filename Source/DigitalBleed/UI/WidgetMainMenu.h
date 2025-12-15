// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidgetMainMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	// 로고 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* LogoImage;

	// UI 버튼들
	UPROPERTY(meta = (BindWidget))
	class UButton* BtnLoadGame;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnNewGame;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnOptions;

	UPROPERTY(meta = (BindWidget))
	class UButton* BtnQuitGame;

protected:
	// 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnBtnLoadGameClicked();

	UFUNCTION()
	void OnBtnNewGameClicked();

	UFUNCTION()
	void OnBtnOptionsClicked();

	UFUNCTION()
	void OnBtnQuitGameClicked();

private:
	// 버튼 이벤트 바인딩
	void BindButtonEvents();
};
