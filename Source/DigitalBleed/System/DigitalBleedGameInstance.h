// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerState.h"
#include "Engine/GameInstance.h"
#include "UI/WidgetMainMenu.h"
#include "DigitalBleedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UDigitalBleedGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UUserWidget* WbpLoadingScreen = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnAir|UI")
	TSubclassOf<UUserWidget> WbpLoadingScreenClass;

	UPROPERTY()
	UWidgetMainMenu* WbpMainMenu = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnAir|UI")
	TSubclassOf<UWidgetMainMenu> WbpMainMenuClass;

	// 맵 스트리밍을 위한 프로퍼티 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming")
	FName LevelToStream = NAME_None;

	UPROPERTY(BlueprintReadOnly, Category = "Level Streaming")
	bool bIsStreaming;

	UPROPERTY(BlueprintReadOnly)
	int32 Month = 3;

	UPROPERTY(BlueprintReadOnly)
	int32 Day = 2;

	UPROPERTY(BlueprintReadOnly)
	int32 Yoil = 3;

	UPROPERTY(BlueprintReadOnly)
	int32 Year = 2016;

	UPROPERTY(BlueprintReadOnly)
	int32 Hour = 0;

	UPROPERTY()
	TObjectPtr<AMyPlayerState> SavedPlayerState;

	UPROPERTY(BlueprintReadOnly)
	int32 GlobalOption_BGMVolume = 50;
	
	UPROPERTY(BlueprintReadOnly)
	int32 GlobalOption_FXVolume = 50;
	
public:
	virtual void InitGamePlayerLoggedIn();
	
	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void StreamMap(FName MapName);
	void ProcessLoadLevel();

	UFUNCTION()
	void OnLevelUnloaded();

	UFUNCTION()
	void OnLevelLoaded();

	UFUNCTION()
	void InitNewGame();

	UFUNCTION(BlueprintCallable)
	void PlayBGM(USoundBase* BGMToPlay);
};
