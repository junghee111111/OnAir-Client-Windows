// Fill out your copyright notice in the Description page of Project Settings.


#include "DigitalBleedGameInstance.h"

#include "Blueprint/UserWidget.h"

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
}
