// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dungeon/DungeonRoom.h"
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
	void GenerateCriticalPath(TArray<int32> PrevPoint, TArray<int32> LastPoint,int32 Length);
	void GenerateBranchPath(TArray<int32> PrevPoint, TArray<int32> LastPoint, int32 Length);
	void GenerateMap();
	void PrintMapInfo();
	void SpawnDungeonRoom(TArray<int32> PrevPoint, TArray<int32> LastPoint,TArray<int32> NextPoint);

	TArray<TArray<ADungeonRoom*>> Rooms = {
		{
				nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
				nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
			},
		{
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
		},
		{
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
		},
		{
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
		},
		{
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
		},
		{
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
		},
		{
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
		},
		{
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
		},
		{
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
		},
		{
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
			nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
		},
{
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
},
{
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
},
{
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
},
{
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
},
{
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
},
{
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
},
{
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
},
{
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
},
{
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
},
{
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr
},
	};
	
	void PostProcess();
	TArray<int32> StartPoint = {0,0};
	TArray<int32> CriticalPathLastPoint = {0,0};
	int32 NumBranches = 0;

	TArray<TArray<int32>> MapData = {
	{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		},
	{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		},
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
},
	};

	TArray<TArray<int32>> DirectionData = {
		{
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
		},
	{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	},
	{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	},
	{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	},
	{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	},
	{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	},
	{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	},
	{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	},
	{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	},
	{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	},{
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
	},
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
},
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
},
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
},
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
},
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
},
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
},
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
},
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
},
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
},
};

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map Object")
	TSubclassOf<ADungeonRoom> MapWallNoneClass = nullptr;

public:
	
	/** Constructor */
	ADigitalBleedGameMode();

	
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
};



