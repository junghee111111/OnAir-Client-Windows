// Fill out your copyright notice in the Description page of Project Settings.


#include "DigitalBleedGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UDigitalBleedGameInstance::InitGamePlayerLoggedIn()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (this->WbpLoadingScreenClass)
	{
		if (!this->WbpLoadingScreen)
		{
			this->WbpLoadingScreen = CreateWidget<class UUserWidget>(PC, this->WbpLoadingScreenClass);
			this->WbpLoadingScreen->AddToViewport();
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("[GameInstance] : No Loading Screen Widget is defined!"));
	}
	this->StreamMap("/Game/Level/Level_Chilgok");
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

	LevelToStream = MapName;
	bIsStreaming = true;

	// 로딩 스크린 표시
	if (WbpLoadingScreen)
	{
		WbpLoadingScreen->SetVisibility(ESlateVisibility::Visible);
	}


	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnLevelLoaded");
	LatentInfo.UUID = GetUniqueID();
	LatentInfo.Linkage = 0;
	UGameplayStatics::LoadStreamLevel(this, LevelToStream, true, true, LatentInfo);
}

void UDigitalBleedGameInstance::OnLevelLoaded()
{
	bIsStreaming = false;
	
	// 로딩 스크린 숨김
	if (WbpLoadingScreen)
	{
		WbpLoadingScreen->SetVisibility(ESlateVisibility::Hidden);
	}

	APlayerController* PC = GetFirstLocalPlayerController();
	if (PC->IsValidLowLevel())
	{
		if (!this->WbpMainMenu && this->WbpMainMenuClass)
		{
			this->WbpMainMenu = CreateWidget<class UWidgetMainMenu>(PC, this->WbpMainMenuClass);
			this->WbpMainMenu->AddToViewport();
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("[GameInstance] : Level streaming completed!"));
}