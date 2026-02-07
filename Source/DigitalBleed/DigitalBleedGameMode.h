// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DigitalBleedGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class ADigitalBleedGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	void GenerateBranches();
	void GenerateCriticalPath(TArray<int32> LastPoint,int32 Length);
	void GenerateBranchPath(TArray<int32> LastPoint,int32 Length);
	void GenerateMap();
	void PrintMapInfo();
	
	void SpawnMap();
	TArray<int32> StartPoint = {0,0};
	TArray<int32> CriticalPathLastPoint = {0,0};
	int32 NumBranches = 0;

	TArray<TArray<int32>> MapData = {
	{
			0,0,0,0,0,0,0,0,0,0
		},
	{
			0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0
		},
	};

	TArray<TArray<int32>> DirectionData = {
		{
			0,0,0,0,0,0,0,0,0,0
		},
	{
		0,0,0,0,0,0,0,0,0,0
	},
	{
		0,0,0,0,0,0,0,0,0,0
	},
	{
		0,0,0,0,0,0,0,0,0,0
	},
	{
		0,0,0,0,0,0,0,0,0,0
	},
	{
		0,0,0,0,0,0,0,0,0,0
	},
	{
		0,0,0,0,0,0,0,0,0,0
	},
	{
		0,0,0,0,0,0,0,0,0,0
	},
	{
		0,0,0,0,0,0,0,0,0,0
	},
	{
		0,0,0,0,0,0,0,0,0,0
	},
};

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map Object")
	TSubclassOf<AActor> MapWallNoneClass = nullptr;

public:
	
	/** Constructor */
	ADigitalBleedGameMode();

	
	virtual void BeginPlay() override;
};



