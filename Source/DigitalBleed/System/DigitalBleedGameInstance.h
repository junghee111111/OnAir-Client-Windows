// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerState.h"
#include "Data/StructModal.h"
#include "Engine/GameInstance.h"
#include "UI/WidgetLoadingScreen.h"
#include "UI/WidgetMainMenu.h"
#include "UI/WidgetModal.h"
#include "DigitalBleedGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGlobalEvent, FString, StringParameter);

UCLASS()
class DIGITALBLEED_API UDigitalBleedGameInstance : public UGameInstance
{
	GENERATED_BODY()

	void FakeLoadingScreenInit();
	
protected:
	UPROPERTY()
	UWidgetLoadingScreen* WbpLoadingScreen = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnAir|UI")
	TSubclassOf<UWidgetLoadingScreen> WbpLoadingScreenClass;

	UPROPERTY()
	UWidgetLoadingScreen* WbpLoadingScreenFaker = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnAir|UI")
	TSubclassOf<UWidgetLoadingScreen> WbpLoadingScreenFakerClass;

	UPROPERTY()
	UWidgetMainMenu* WbpMainMenu = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnAir|UI")
	TSubclassOf<UWidgetMainMenu> WbpMainMenuClass;

	UPROPERTY()
	UWidgetModal* WbpModal = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnAir|UI")
	TSubclassOf<UWidgetModal> WbpModalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* DT_Modal = nullptr;

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
	virtual void Init() override;
	

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);

	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGlobalEvent OnGlobalEvent;

	UFUNCTION()
	void DoGlobalEvent(FString StringParameter);
	
	virtual void InitGamePlayerLoggedIn();

	UFUNCTION(BlueprintCallable)
	void JustOpenMap(FName MapName);

	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void StreamMap(FName MapName);
	void ProcessLoadLevel();

	UFUNCTION()
	void OnLevelUnloaded();

	UFUNCTION()
	void OnLevelLoaded();

	UFUNCTION()
	void ShowNewGameModal();

	UFUNCTION(BlueprintCallable)
	void PlayBGM(USoundBase* BGMToPlay);

	UFUNCTION(BlueprintCallable)
	void ShowModal(FRowModal Modal);

	UFUNCTION(BlueprintCallable)
	void HideModal();
};
