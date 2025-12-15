// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GMMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API AGMMainMenu : public AGameMode
{
	GENERATED_BODY()

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
