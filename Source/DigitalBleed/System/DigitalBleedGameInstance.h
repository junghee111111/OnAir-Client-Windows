// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerState.h"
#include "Data/StructDialog.h"
#include "Data/StructModal.h"
#include "Engine/GameInstance.h"
#include "UI/WidgetDialog.h"
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

	UPROPERTY()
	UWidgetDialog* WbpDialog = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnAir|UI")
	TSubclassOf<UWidgetDialog> WbpDialogClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* DT_Modal = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* DT_Dialog = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStringTable* ST_UI = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Level Streaming")
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

	// ==============================
	// SOUNDS
	// ==============================
	
	UPROPERTY()
	UAudioComponent* CurrentBGMAudioComponent = nullptr;

	UPROPERTY()
	UAudioComponent* CurrentDialogAudioComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 GlobalOption_BGMVolume = 50;
	
	UPROPERTY(BlueprintReadOnly)
	int32 GlobalOption_FXVolume = 70;

	UPROPERTY(BlueprintReadOnly)
	int32 GlobalOption_DialogVolume = 100;
	
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	AMyPlayerState* GetSavedPlayerState() const { return SavedPlayerState; }
	
	UFUNCTION(BlueprintCallable)
	int32 GetYear() const { return Year; }

	UFUNCTION(BlueprintCallable)
	int32 GetMonth() const { return Month; }

	UFUNCTION(BlueprintCallable)
	int32 GetDay() const { return Day; }

	UFUNCTION(BlueprintCallable)
	int32 GetHour() const { return Hour; }

	UFUNCTION(BlueprintCallable)
	int32 GetYoil() const { return Yoil; }

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
	void PlayDialogSound(USoundBase* DialogSound);

	UFUNCTION(BlueprintCallable)
	void ShowModal(FRowModal Modal);

	UFUNCTION(BlueprintCallable)
	void HideModal();

	UFUNCTION(BlueprintCallable)
	void ShowDialog(FRowDialog Modal);

	UFUNCTION(BlueprintCallable)
	void HideDialog();

	UFUNCTION(BlueprintCallable)
	FRowDialog FindDialogByRowName(FName Name);

	UFUNCTION(BlueprintCallable)
	FString GetUIString(FText RowKey);
};
