// Fill out your copyright notice in the Description page of Project Settings.


#include "GMMainMenu.h"

#include "DigitalBleedGameInstance.h"

void AGMMainMenu::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	Super::PostActorCreated();

	UDigitalBleedGameInstance* GI = Cast<UDigitalBleedGameInstance>(GetGameInstance());
	if (IsValid(GI))
	{
		GI->InitGamePlayerLoggedIn();
	}
}
