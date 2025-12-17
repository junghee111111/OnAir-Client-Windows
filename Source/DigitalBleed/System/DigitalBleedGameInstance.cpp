// Fill out your copyright notice in the Description page of Project Settings.


#include "DigitalBleedGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Data/StructModal.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetModal.h"
#include "MoviePlayer.h"

void UDigitalBleedGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UDigitalBleedGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UDigitalBleedGameInstance::EndLoadingScreen);

	this->FakeLoadingScreenInit();
}

void UDigitalBleedGameInstance::FakeLoadingScreenInit()
{
	//Faker init.
	if (this->WbpLoadingScreenFakerClass)
	{
		if (!this->WbpLoadingScreenFaker)
		{
			this->WbpLoadingScreenFaker = CreateWidget<class UWidgetLoadingScreen>(
				this, this->WbpLoadingScreenFakerClass, TEXT("LoadingScreenFaker"));
			this->WbpLoadingScreenFaker->AddToViewport();
		}
	}
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
	if (InLoadedWorld)
	{
		FTimerHandle TimerHandle;
		this->FakeLoadingScreenInit();
		InLoadedWorld->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
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
		this->JustOpenMap("/Game/Level/Level_Chilgok");
	}
}

void UDigitalBleedGameInstance::InitGamePlayerLoggedIn()
{
	OnGlobalEvent.AddDynamic(this, &UDigitalBleedGameInstance::DoGlobalEvent);
	//this->StreamMap("/Game/Level/Level_Chilgok");
}

void UDigitalBleedGameInstance::JustOpenMap(FName MapName)
{
	this->WbpLoadingScreenFaker->Show();
	FTimerHandle TimerHandle;
	this->GetTimerManager().SetTimer(TimerHandle, [this,MapName]()
	{
		UGameplayStatics::OpenLevel(this, MapName);
	}, 2.0f, false);
	
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
			// Set up latent action for unload completion
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
	if (!BGMToPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GameInstance] : Invalid BGM sound provided!"));
		return;
	}

	// 볼륨 계산 (0-100 범위를 0.0-1.0으로 변환)
	float VolumeMultiplier = GlobalOption_BGMVolume / 100.0f;

	// BGM 재생 (2D 사운드로, 루프 설정)
	UGameplayStatics::PlaySound2D(this, BGMToPlay, VolumeMultiplier);
}

void UDigitalBleedGameInstance::ShowModal(FRowModal Modal)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC->IsValidLowLevel())
	{
		if (!this->WbpModal && this->WbpModalClass)
		{
			this->WbpModal = CreateWidget<class UWidgetModal>(PC, this->WbpModalClass);
			this->WbpModal->SetModalData(Modal);
			this->WbpModal->AddToViewport(50);
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
