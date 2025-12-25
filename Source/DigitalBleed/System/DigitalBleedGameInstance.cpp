// Fill out your copyright notice in the Description page of Project Settings.


#include "DigitalBleedGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Data/StructModal.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetModal.h"
#include "MoviePlayer.h"
#include "Components/AudioComponent.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Runtime/LevelSequence/Public/LevelSequenceDirector.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h" 

constexpr int32 Z_INDEX_LOADING_SCREEN_FAKER = 100;
constexpr int32 Z_INDEX_MODAL = 50;
constexpr int32 Z_INDEX_DIALOG = 40;
constexpr int32 Z_INDEX_DIALOG_SELECTION = 45;
constexpr int32 Z_INDEX_MAIN_HUD = 30;

void UDigitalBleedGameInstance::FakeLoadingScreenInit()
{
	//Faker init.
	if (this->WbpLoadingScreenFakerClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GameInstance] : Loading screen faker created."));
		APlayerController* PC = GetFirstLocalPlayerController();
		this->WbpLoadingScreenFaker = CreateWidget<class UWidgetLoadingScreen>(
			PC, this->WbpLoadingScreenFakerClass, TEXT("LoadingScreenFaker"));
		this->WbpLoadingScreenFaker->AddToViewport(Z_INDEX_LOADING_SCREEN_FAKER);
	}
}

void UDigitalBleedGameInstance::Init()
{
	Super::Init();
	SavedPlayerState = NewObject<AMyPlayerState>(this);
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UDigitalBleedGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UDigitalBleedGameInstance::EndLoadingScreen);

}

void UDigitalBleedGameInstance::SetGlobalOption_BGMVolume(int32 Volume)
{
	this->GlobalOption_BGMVolume = Volume;
}

// ==============================
// Global Level Sequence Director
// ==============================
void UDigitalBleedGameInstance::SetLevelSequenceDirector(ULevelSequenceDirector* NewDirector)
{
	this->LevelSequenceDirector = NewDirector;
}

void UDigitalBleedGameInstance::ResetLevelSequenceDirector()
{
	this->LevelSequenceDirector = nullptr;
}

void UDigitalBleedGameInstance::BeginLoadingScreen(const FString& MapName)
{
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] : Check if dedicated server running"));
	if (IsRunningDedicatedServer()) return;
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] : Loading screen started."));
	
	
	if (this->WbpLoadingScreenClass)
	{
		if (!this->WbpLoadingScreen)
		{
			this->WbpLoadingScreen = CreateWidget<class UWidgetLoadingScreen>(
				this, this->WbpLoadingScreenClass, TEXT("LoadingScreen"));
		}
		FLoadingScreenAttributes LoadingScreenAttr;
		LoadingScreenAttr.WidgetLoadingScreen = this->WbpLoadingScreen->TakeWidget();
		LoadingScreenAttr.bAllowInEarlyStartup = false;
		LoadingScreenAttr.PlaybackType = MT_Normal;
		LoadingScreenAttr.bAllowEngineTick = false;
		LoadingScreenAttr.bWaitForManualStop = false;
		LoadingScreenAttr.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreenAttr.MinimumLoadingScreenDisplayTime = 1.f;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttr);
	}
}

void UDigitalBleedGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] : Loading screen ended."));
	if (InLoadedWorld)
	{
		FTimerHandle TimerHandle;
		this->FakeLoadingScreenInit();
		UE_LOG(LogTemp, Log, TEXT("[GameInstance] : Loading screen try to vanish.."));
		InLoadedWorld->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			UE_LOG(LogTemp, Log, TEXT("[GameInstance] : Loading screen has been vanished."));
			this->WbpLoadingScreenFaker->Hide();
		}, 2.0f, false);
	}
}

void UDigitalBleedGameInstance::DoGlobalEvent(FString StringParameter)
{
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] : Global Event : %s"), *StringParameter);
	if (FString(StringParameter) == "InitNewGame")
	{
		// 새로운 PlayerState 생성
		SavedPlayerState = NewObject<AMyPlayerState>(this);

		if (SavedPlayerState)
		{
			// PlayerState 초기화 (생성자에서 기본값이 설정되지만 추가 설정 가능)
			UE_LOG(LogTemp, Log, TEXT("New PlayerState created successfully"));
		}
		this->JustOpenMap("/Game/Level/Level_House");
	} else if (StringParameter == "ProceedCycle")
	{
		if (WbpCycleTransitionClass->IsValidLowLevel())
		{
			if (!WbpCycleTransition)
			{
				this->WbpCycleTransition = CreateWidget<class UWidgetCycleTransition>(
					this, WbpCycleTransitionClass, TEXT("CycleTransition"));
			}
			// 씨발 이렇게 안하면 반복적으로 안뜨네
			if (!WbpCycleTransition->IsInViewport()) WbpCycleTransition->AddToViewport(Z_INDEX_LOADING_SCREEN_FAKER);
			WbpCycleTransition->PlayCycleTransitionAnim();

			FTimerHandle Th;
			this->GetTimerManager().SetTimer(Th, [this]()
			{
				this->Hour++;
				if (this->WbpMainHud->IsValidLowLevel())
				{
					this->WbpMainHud->UpdateHud();
				}
			},1.0f, false);
		}
	}
}

void UDigitalBleedGameInstance::ShowMainHud()
{
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] : Show Main HUD"));
	if (WbpMainHudClass->IsValidLowLevel())
	{
		if (!WbpMainHud->IsValidLowLevel())
		{
			WbpMainHud = CreateWidget<class UWidgetMainHud>(
				this, WbpMainHudClass, TEXT("MainHud"));
		}
		if (!WbpMainHud->IsInViewport())
		{
			WbpMainHud->AddToViewport(Z_INDEX_MAIN_HUD);
		}
		WbpMainHud->UpdateHud();
		WbpMainHud->PlayShowAnim();
	}
}

void UDigitalBleedGameInstance::HideMainHud()
{
	if (WbpMainHudClass->IsValidLowLevel())
	{
		if (WbpMainHud->IsValidLowLevel())
		{
			WbpMainHud->UpdateHud();
			WbpMainHud->PlayHideAnim();
		}
	}
}

void UDigitalBleedGameInstance::InitGamePlayerLoggedIn()
{
	OnGlobalEvent.AddDynamic(this, &UDigitalBleedGameInstance::DoGlobalEvent);
	//this->StreamMap("/Game/Level/Level_Chilgok");
}

void UDigitalBleedGameInstance::JustOpenMap(FName MapName)
{
	this->FakeLoadingScreenInit();
	this->WbpLoadingScreenFaker->Show();
	FTimerHandle TimerHandle;
	this->GetTimerManager().SetTimer(TimerHandle, [this,MapName]()
	{
		UGameplayStatics::OpenLevel(this, MapName);
	}, 1.0f, false);
	
}

void UDigitalBleedGameInstance::StreamMap(FName MapName)
{
	if (MapName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("[GameInstance] : Invalid map name provided for streaming!"));
		return;
	}

	if (bIsStreaming)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GameInstance] : Already streaming a level!"));
		return;
	}
	
	// 로딩 스크린 표시
	if (WbpLoadingScreen)
	{
		WbpLoadingScreen->Show();
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this,MapName]()
	{
		//delay 1 second
		if (!this->LevelToStream.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("Try to unload existing level..."));
			FLatentActionInfo UnloadLatentInfo;
			UnloadLatentInfo.CallbackTarget = this;
			UnloadLatentInfo.ExecutionFunction = FName("OnLevelUnloaded");
			UnloadLatentInfo.UUID = GetUniqueID();
			UnloadLatentInfo.Linkage = 0;
			
			UGameplayStatics::UnloadStreamLevel(this, LevelToStream, UnloadLatentInfo, true);
			this->LevelToStream = MapName;
			return;
		}

		this->LevelToStream = MapName;
		this->ProcessLoadLevel();
	}, 1.0f, false);

	
}

void UDigitalBleedGameInstance::SetNextPos(FString newPos)
{
	this->NextPos = newPos;
}

void UDigitalBleedGameInstance::SetNextSeq(FString newSeq)
{
	this->NextSeq = newSeq;
}

FString UDigitalBleedGameInstance::GetCycleText(int32 Cycle)
{
	switch (Cycle)
	{
	case 0: return this->GetUIString(FText::FromString("HUD_MORNING"));
	case 1: return this->GetUIString(FText::FromString("HUD_BRUNCH"));
	case 2: return this->GetUIString(FText::FromString("HUD_LUNCH"));
	case 3: return this->GetUIString(FText::FromString("HUD_EVENING"));
	case 4: return this->GetUIString(FText::FromString("HUD_AFTERSCHOOL"));
	case 5: return this->GetUIString(FText::FromString("HUD_DINNER"));
	case 6: return this->GetUIString(FText::FromString("HUD_NIGHT"));
	default: return "";
	}
}

void UDigitalBleedGameInstance::ProcessLoadLevel()
{
	bIsStreaming = true;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnLevelLoaded");
	LatentInfo.UUID = GetUniqueID();
	LatentInfo.Linkage = 0;
	UGameplayStatics::LoadStreamLevel(this, LevelToStream, true, true, LatentInfo);
}

void UDigitalBleedGameInstance::OnLevelUnloaded()
{
	UE_LOG(LogTemp, Log, TEXT("[GameInstance] : Level Unloaded."));
	if (!this->LevelToStream.IsNone())
	{
		this->ProcessLoadLevel();
	}
}

void UDigitalBleedGameInstance::OnLevelLoaded()
{
	bIsStreaming = false;
	
	// 로딩 스크린 숨김
	if (WbpLoadingScreen)
	{
		WbpLoadingScreen->Hide();
	}
	
	UE_LOG(LogTemp, Log, TEXT("[GameInstance] : Level streaming completed!"));
}

void UDigitalBleedGameInstance::ShowNewGameModal()
{
	if (DT_Modal)
	{
		FRowModal* ModalData = DT_Modal->FindRow<FRowModal>(FName("ModalNewGame"), TEXT(""));
		this->ShowModal(*ModalData);
	}
}

void UDigitalBleedGameInstance::PlayBGM(USoundBase* BGMToPlay)
{
	//UE_LOG(LogTemp, Warning, TEXT("[GameInstance::PlayBGM] : Function Called!"));
	if (!BGMToPlay)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[GameInstance::PlayBGM] : Invalid BGM sound provided!"));
		return;
	}

	// Null 체크 추가 및 재생 중인지 확인
	if (CurrentBGMAudioComponent && CurrentBGMAudioComponent->IsValidLowLevel() && 
		CurrentBGMAudioComponent->Sound == BGMToPlay && 
		CurrentBGMAudioComponent->IsPlaying())
	{
		UE_LOG(LogTemp, Warning, TEXT("[GameInstance::PlayBGM] : BGM is same.. Ignored!"));
		return;
	}

	// BGM 페이드 아웃
	if (CurrentBGMAudioComponent && CurrentBGMAudioComponent->IsPlaying())
	{
		// 1초 동안 페이드 아웃
		//UE_LOG(LogTemp, Warning, TEXT("[GameInstance::PlayBGM] : Fade out previous BGM."));
		CurrentBGMAudioComponent->FadeOut(1.0f, 0.0f);
	}

	// 볼륨 계산 (0-100 범위를 0.0-1.0으로 변환)
	
	
	
	FTimerHandle Th;
	//(LogTemp, Warning, TEXT("[GameInstance::PlayBGM] : Play BGM shit"));
	this->GetTimerManager().SetTimer(Th, [this,BGMToPlay]()
	{
		UE_LOG(LogTemp, Warning, TEXT("[GameInstance::PlayBGM] : Play BGM Callback function shit called."));
		float VolumeMultiplier = this->GlobalOption_BGMVolume / 100.0f;
		CurrentBGMAudioComponent = UGameplayStatics::SpawnSound2D(this, BGMToPlay, VolumeMultiplier, 1.0f, 0.0f, nullptr, true, false);
		if (CurrentBGMAudioComponent) this->CurrentBGMAudioComponent->Play();
	},1.0f, false);
}

void UDigitalBleedGameInstance::PlayDialogSound(USoundBase* DialogSound)
{
	if (!DialogSound || !DialogSound->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("[GameInstance] : Invalid Dialog sound provided!"));
		return;
	}

	// 기존 BGM이 재생 중이면 정지
	if (CurrentDialogAudioComponent && CurrentDialogAudioComponent->IsPlaying())
	{
		CurrentDialogAudioComponent->Stop();
	}

	// 볼륨 계산 (0-100 범위를 0.0-1.0으로 변환)
	float VolumeMultiplier = GlobalOption_DialogVolume / 100.0f;

	// BGM 재생 (2D 사운드로, 루프 설정) - AudioComponent 반환받기
	CurrentDialogAudioComponent = UGameplayStatics::SpawnSound2D(this, DialogSound, VolumeMultiplier, 1.0f, 0.0f, nullptr, true, false);
	
	if (CurrentDialogAudioComponent)
	{
		CurrentDialogAudioComponent->Play();
	}
}

// ==============================
// Global UI Controls
// ==============================
void UDigitalBleedGameInstance::ShowModal(FRowModal Modal)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC->IsValidLowLevel())
	{
		if (!this->WbpModal && this->WbpModalClass)
		{
			this->WbpModal = CreateWidget<class UWidgetModal>(PC, this->WbpModalClass);
			this->WbpModal->SetModalData(Modal);
			this->WbpModal->AddToViewport(Z_INDEX_MODAL);
		}
	}
}

void UDigitalBleedGameInstance::HideModal()
{
	if (this->WbpModal->IsInViewport())
	{
		this->WbpModal->RemoveFromParent();
		this->WbpModal = nullptr;
	}
}

void UDigitalBleedGameInstance::ShowDialog(FRowDialog Dialog)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC->IsValidLowLevel())
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
		if (!this->WbpDialog && this->WbpDialogClass)
		{
			this->WbpDialog = CreateWidget<class UWidgetDialog>(PC, this->WbpDialogClass);
		}
		if (!this->WbpDialog->IsInViewport())
		{
			this->WbpDialog->AddToViewport(Z_INDEX_DIALOG);
		}
		if (this->WbpDialogSelection->IsValidLowLevel() && this->WbpDialogSelection->IsInViewport())
		{
			this->HideDialogSelection();
		}
		this->WbpDialog->SetDialogData(Dialog);
		if (this->LevelSequenceDirector->IsValidLowLevel())
		{
			this->LevelSequenceDirector->Player->Pause();
		}
	}
}

void UDigitalBleedGameInstance::HideDialog()
{
	if (this->WbpDialog->IsInViewport())
	{
		this->WbpDialog->RemoveFromParent();
		this->WbpDialog = nullptr;

		APlayerController* PC = GetFirstLocalPlayerController();
		if (PC)
		{
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}
		if (this->LevelSequenceDirector->IsValidLowLevel())
		{
			this->LevelSequenceDirector->Player->Play();
		}
	}
}

void UDigitalBleedGameInstance::ShowDialogSelection(FRowSelection Selection)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC->IsValidLowLevel())
	{
		if (!this->WbpDialogSelection && this->WbpDialogSelectionClass)
		{
			this->WbpDialogSelection = CreateWidget<class UWidgetDialogSelection>(PC, this->WbpDialogSelectionClass);
			this->WbpDialogSelection->SetSelectionData(Selection);
		}
		if (!this->WbpDialogSelection->IsInViewport())
		{
			this->WbpDialogSelection->AddToViewport(Z_INDEX_DIALOG_SELECTION);
		}
	}
}

void UDigitalBleedGameInstance::HideDialogSelection()
{
	if (this->WbpDialogSelection->IsInViewport())
	{
		this->WbpDialogSelection->RemoveFromParent();
		this->WbpDialogSelection = nullptr;
	}
}

FRowDialog UDigitalBleedGameInstance::FindDialogByRowName(FName Name)
{
	if (!this->DT_Dialog) return *(new FRowDialog());
	return *DT_Dialog->FindRow<FRowDialog>(Name, TEXT(""));
}

FRowSelection UDigitalBleedGameInstance::FindDialogSelectionByRowName(FName Name)
{
	if (!this->DT_Selection) return *(new FRowSelection());
	return *DT_Selection->FindRow<FRowSelection>(Name, TEXT(""));
}

FString UDigitalBleedGameInstance::GetUIString(FText RowKey)
{
	if (!ST_UI)
	{
		UE_LOG(LogTemp, Error, TEXT("[GameInstance] : ST_UI is not assigned!"));
		return FString(TEXT(""));
	}

	FStringTableEntryConstPtr StringData = ST_UI->GetStringTable()->FindEntry(FTextKey(
		RowKey.ToString()
	));
	
	if (StringData.IsValid())
	{
		return StringData->GetSourceString();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] : Row '%s' not found in ST_UI!"), *RowKey.ToString());
	return FString(TEXT(""));
}
