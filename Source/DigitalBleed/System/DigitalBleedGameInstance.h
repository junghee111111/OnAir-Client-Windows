// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DigitalBleedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UDigitalBleedGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	int32 Month = 3;
	int32 Day = 2;
	int32 Yoil = 3;
	int32 Year = 2016;
	int32 Hour = 0;

protected:
	UPROPERTY()
	UUserWidget* WbpLoadingScreen = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OnAir|UI")
	TSubclassOf<UUserWidget> WbpLoadingScreenClass;

public:
	virtual void InitGamePlayerLoggedIn();
};
