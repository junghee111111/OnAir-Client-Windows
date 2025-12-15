// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
<<<<<<< HEAD
#include "UI/WidgetMainMenu.h"
=======
>>>>>>> bebc240abf8aec06802bfe897c1d6c7e0b995438
#include "DigitalBleedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UDigitalBleedGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	int32 Month = 3;
	int32 Day = 2;
	int32 Yoil = 3;
	int32 Year = 2016;
	int32 Hour = 0;

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
	FName LevelToStream;

	UPROPERTY(BlueprintReadOnly, Category = "Level Streaming")
	bool bIsStreaming;

public:
	virtual void InitGamePlayerLoggedIn();
	
	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void StreamMap(FName MapName);

	UFUNCTION()
	void OnLevelLoaded();
};
