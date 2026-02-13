// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Battle/LifeHuman.h"
#include "Dungeon/DungeonRoom.h"
#include "GameFramework/GameModeBase.h"
#include "System/DigitalBleedGameInstance.h"
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
	void SpawnDungeonRoom(TArray<int32> PrevPoint, TArray<int32> CurrentPoint, bool bIsBranch);
	void RestoreDungeonData();
	void SpawnDungeonFromRestoredData();

	TArray<TArray<ADungeonRoom*>> Rooms;
	TArray<TArray<int32>> MapData;
	TArray<TArray<int32>> DirectionData;
	TArray<TArray<int32>> PrevDirectionData;
	TArray<TArray<int32>> BranchData;
	
	void PostProcess();
	TArray<int32> StartPoint = {0,0};
	TArray<int32> CriticalPathLastPoint = {0,0};
	int32 NumBranches = 0;

	UDigitalBleedGameInstance* MyGameInstance = nullptr;
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map Object")
	TSubclassOf<ADungeonRoom> MapWallNoneClass = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Classes")
	TSubclassOf<AActor> Player_HYJClass = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Classes")
	TSubclassOf<AActor> Player_CJYClass = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Classes")
	TSubclassOf<AActor> Player_JARClass = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Classes")
	TSubclassOf<AActor> Player_PMSClass = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Classes")
	TSubclassOf<AActor> Player_YJSClass = nullptr;

	UPROPERTY()
	TArray<AActor*> PartyMembers;


public:
	
	/** Constructor */
	ADigitalBleedGameMode();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void SpawnPlayers(const FTransform& SpawnTransform);
};



