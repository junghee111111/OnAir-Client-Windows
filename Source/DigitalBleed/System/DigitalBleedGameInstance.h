// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceDirector.h"
#include "MyPlayerState.h"
#include "Data/StructDialog.h"
#include "Data/StructItem.h"
#include "Data/StructModal.h"
#include "Data/StructSkill.h"
#include "Engine/GameInstance.h"
#include "UI/WidgetCycleTransition.h"
#include "UI/WidgetDayTransition.h"
#include "UI/WidgetDialog.h"
#include "UI/WidgetDialogSelection.h"
#include "UI/WidgetGlobalToast.h"
#include "UI/WidgetLoadingScreen.h"
#include "UI/WidgetMainMenu.h"
#include "UI/WidgetModal.h"
#include "UI/WidgetTriage.h"
#include "UI/MainHud/WidgetMainHud.h"
#include "DigitalBleedGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGlobalEvent, FString, StringParameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCycleChanged, FString, StringParameter);

UCLASS()
class DIGITALBLEED_API UDigitalBleedGameInstance : public UGameInstance
{
	GENERATED_BODY()

	void FakeLoadingScreenInit();
	TArray<int32> CalculateNextDay() const;
	
protected:
	UPROPERTY()
	UWidgetLoadingScreen* WbpLoadingScreen = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetLoadingScreen> WbpLoadingScreenClass;

	UPROPERTY()
	UWidgetMainHud* WbpMainHud = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetMainHud> WbpMainHudClass;

	UPROPERTY()
	UWidgetLoadingScreen* WbpLoadingScreenFaker = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetLoadingScreen> WbpLoadingScreenFakerClass;

	UPROPERTY()
	UWidgetMainMenu* WbpMainMenu = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetMainMenu> WbpMainMenuClass;

	UPROPERTY()
	UWidgetModal* WbpModal = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetModal> WbpModalClass;

	UPROPERTY()
	UWidgetCycleTransition* WbpCycleTransition = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetCycleTransition> WbpCycleTransitionClass;

	UPROPERTY()
	UWidgetDayTransition* WbpDayTransition = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetDayTransition> WbpDayTransitionClass;
	
	UPROPERTY()
	UWidgetDialog* WbpDialog = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetDialog> WbpDialogClass;

	UPROPERTY()
	UWidgetDialogSelection* WbpDialogSelection = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetDialogSelection> WbpDialogSelectionClass;

	UPROPERTY()
	UWidgetGlobalToast* WbpGlobalToast = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetGlobalToast> WbpGlobalToastClass;

	UPROPERTY()
	UUserWidget* WbpTransitionInitMatrix = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUserWidget> WbpTransitionInitMatrixClass;

	UPROPERTY()
	UWidgetTriage* WbpTriage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UWidgetTriage> WbpTriageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* DT_Modal = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* DT_Selection = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* DT_Dialog = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* DT_PartyMembers = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* DT_ExpTable = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStringTable* ST_UI = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStringTable* ST_Skill = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStringTable* ST_Item = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Party")
	TArray<FString> PartyIn;

	UPROPERTY(BlueprintReadWrite, Category = "Party")
	TArray<FString> PartyOut;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UserData")
	TArray<FRowSkillRecord> SkillRecords;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UserData")
	TArray<FRowItemRecord> ItemRecords;

	UPROPERTY(BlueprintReadOnly)
	int32 Money_Won = 0;

	UPROPERTY(BlueprintReadOnly)
	float Money_Btc = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Level Streaming")
	FName LevelToStream = NAME_None;

	UPROPERTY(BlueprintReadOnly)
	FString NextPos = "0";

	UPROPERTY(BlueprintReadOnly)
	FString NextSeq = "";

	UPROPERTY(BlueprintReadOnly, Category = "Level Streaming")
	bool bIsStreaming;

	UPROPERTY(BlueprintReadOnly, Category = "Level Streaming")
	bool bIsOpeningMap;

	UPROPERTY(BlueprintReadOnly)
	bool bNeedsRestoreMatrix = false;

	UPROPERTY(BlueprintReadOnly)
	int32 Month = 3;

	UPROPERTY(BlueprintReadOnly)
	int32 Day = 3;

	UPROPERTY(BlueprintReadOnly)
	int32 Yoil = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 Year = 2014;

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

	UPROPERTY(BlueprintReadOnly)
	ULevelSequenceDirector* LevelSequenceDirector = nullptr;
	
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

	UFUNCTION(BlueprintCallable)
	int32 GetPartyLength() const { return PartyIn.Num(); }

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetPartyMembers() const { return PartyIn; }

	UFUNCTION(BlueprintCallable)
	void SetGlobalOption_BGMVolume(int32 Volume);

	UFUNCTION(BlueprintCallable)
	void SetLevelSequenceDirector(ULevelSequenceDirector* NewDirector);

	UFUNCTION(BlueprintCallable)
	void ResetLevelSequenceDirector();

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);

	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGlobalEvent OnGlobalEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGlobalEvent OnCycleChanged;

	UFUNCTION(BlueprintCallable)
	void DoGlobalEvent(FString StringParameter);

	UFUNCTION(BlueprintCallable)
	void ShowMainHud();

	UFUNCTION(BlueprintCallable)
	void HideMainHud();

	virtual void InitGamePlayerLoggedIn();

	UFUNCTION(BlueprintCallable)
	void JustOpenMap(FName MapName);

	UFUNCTION(BlueprintCallable, Category = "Level Streaming")
	void StreamMap(FName MapName);

	UFUNCTION(BlueprintCallable)
	FString GetNextPos();

	UFUNCTION(BlueprintCallable)
	void SetNextPos(FString newPos);

	UFUNCTION(BlueprintCallable)
	void SetNextSeq(FString newSeq);

	UFUNCTION(BlueprintCallable)
	FString GetCycleText(int32 Cycle);
	FText GetTodayText();
	FText GetTommorowText();

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
	void PlaySFX(USoundBase* SFXToPlay);

	UFUNCTION(BlueprintCallable)
	void FadeOutBGM() const;
	
	void PlayDialogSound(USoundBase* DialogSound);

	UFUNCTION(BlueprintCallable)
	void ShowModal(FRowModal Modal);

	UFUNCTION(BlueprintCallable)
	void HideModal();

	UFUNCTION(BlueprintCallable)
	void ShowDialog(FRowDialog Dialog);

	UFUNCTION(BlueprintCallable)
	void HideDialog();

	UFUNCTION(BlueprintCallable)
	void ShowDialogSelection(FRowSelection Selection);

	UFUNCTION(BlueprintCallable)
	void HideDialogSelection();
	
	UFUNCTION(BlueprintCallable)
	FRowDialog FindDialogByRowName(FName Name);

	UFUNCTION(BlueprintCallable)
	FRowSelection FindDialogSelectionByRowName(FName Name);

	UFUNCTION(BlueprintCallable)
	FString GetUIString(FText RowKey);

	UFUNCTION(BlueprintCallable)
	FString GetSkillString(FText RowKey);

	UFUNCTION(BlueprintCallable)
	void AddPartyMember(const FString& MemberID);

	UFUNCTION(BlueprintCallable)
	void RemovePartyMember(const FString& MemberID);

	UFUNCTION(BlueprintCallable)
	bool IsInParty(const FString& MemberID) const;

	UFUNCTION(BlueprintCallable)
	void ShowToast(FText RowKey);

	UFUNCTION(BlueprintCallable)
	void ShowToastSkillName(FText RowKey);

	UFUNCTION(BlueprintCallable)
	FString GetItemString(const FText& ItemId);
	
	UFUNCTION(BlueprintCallable)
	void ShowToastItemName(FText RowKey);

	// 레벨에 필요한 경험치 조회
	UFUNCTION(BlueprintCallable)
	int32 GetMaxExpForLevel(int32 Level);

	UFUNCTION(BlueprintCallable)
	void AddMoneyWon(int32 Delta);

	UFUNCTION(BlueprintCallable)
	void AddMoneyBtc(float Delta);

	// Item Management
	UFUNCTION(BlueprintCallable)
	bool AddItem(FName ItemId, int32 Qty = 1);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(FName ItemId, int32 Qty = 1);

	UFUNCTION(BlueprintCallable)
	int32 GetItemQuantity(FName ItemId) const;

	UFUNCTION(BlueprintCallable)
	bool HasItem(FName ItemId, int32 MinQty = 1) const;

	UFUNCTION(BlueprintCallable)
	float GetMoneyBtc() const { return Money_Btc; }

	UFUNCTION(BlueprintCallable)
	int32 GetMoneyWon() const { return Money_Won; }

	UFUNCTION(BlueprintCallable)
	void SetNeedsRestoreMatrix(bool newBool);
};
