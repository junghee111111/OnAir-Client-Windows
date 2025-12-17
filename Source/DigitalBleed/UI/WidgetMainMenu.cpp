// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetMainMenu.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/DigitalBleedGameInstance.h"

void UWidgetMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	this->BindButtonEvents();
}

void UWidgetMainMenu::OnBtnLoadGameClicked()
{
}

void UWidgetMainMenu::OnBtnNewGameClicked()
{
	UDigitalBleedGameInstance* GI = Cast<UDigitalBleedGameInstance>(GetGameInstance());
	if (IsValid(GI))
	{
		GI->ShowNewGameModal();
	}
}

void UWidgetMainMenu::OnBtnOptionsClicked()
{
}

void UWidgetMainMenu::OnBtnQuitGameClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Quit Game Button Clicked"));
	
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		// 에디터에서는 시뮬레이션 종료, 패키지 빌드에서는 게임 종료
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
	}
}

void UWidgetMainMenu::BindButtonEvents()
{
	if (BtnLoadGame)
	{
		BtnLoadGame->OnClicked.AddDynamic(this, &UWidgetMainMenu::OnBtnLoadGameClicked);
	}

	if (BtnNewGame)
	{
		BtnNewGame->OnClicked.AddDynamic(this, &UWidgetMainMenu::OnBtnNewGameClicked);
	}

	if (BtnOptions)
	{
		BtnOptions->OnClicked.AddDynamic(this, &UWidgetMainMenu::OnBtnOptionsClicked);
	}

	if (BtnQuitGame)
	{
		BtnQuitGame->OnClicked.AddDynamic(this, &UWidgetMainMenu::OnBtnQuitGameClicked);
	}
}