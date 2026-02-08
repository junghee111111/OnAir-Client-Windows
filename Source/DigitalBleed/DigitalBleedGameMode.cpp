// 26.02.06, Written By Junghee Wang

#include "DigitalBleedGameMode.h"

#include "CborTypes.h"
#include "HLSLTypeAliases.h"

constexpr int32 GDungeonMapWidth = 20;
constexpr int32 GTileWidth = 5;
constexpr float GTileSize = GTileWidth * 100.0;
const FVector GMapRoomStartOffset = FVector(0.0f, 0.0f, 0.0f);
constexpr int32 GCriticalPathLength = 40;
constexpr int32 GBranchesMaxNum = 3;
constexpr int32 GBranchLength = 10;

ADigitalBleedGameMode::ADigitalBleedGameMode()
{
	// stub
}

void ADigitalBleedGameMode::PrintMapInfo()
{
	UE_LOG(LogTemp, Log, TEXT("MAP======================"));
	for (int i = 0; i < GDungeonMapWidth; i++)
	{
		FString Data = "";
		for (int j = 0; j < GDungeonMapWidth; j++)
		{
			Data += FString::FromInt(this->MapData[i][j]) + " ";
		}
		UE_LOG(LogTemp, Log, TEXT("%s"), *Data);
	}
	UE_LOG(LogTemp, Log, TEXT("DIR======================"));
	for (int i = 0; i < GDungeonMapWidth; i++)
	{
		FString Data = "";
		for (int j = 0; j < GDungeonMapWidth; j++)
		{
			Data += FString::FromInt(this->DirectionData[i][j]) + " ";
		}
		UE_LOG(LogTemp, Log, TEXT("%s"), *Data);
	}
}

void ADigitalBleedGameMode::GenerateBranches()
{
	for (int i = 0; i < GDungeonMapWidth; i ++)
	{
		for (int j = 0; j < GDungeonMapWidth; j++)
		{
			if (this->MapData[i][j] == 4)
			{
				this->GenerateBranchPath({i,j}, {i,j}, FMath::RandRange(1, GBranchLength));
			}
		}
	}
}

/**
 * 실제 Map Mesh를 Spawn 한다.
 * @param LastPoint 이전 좌표
 * @param NextPoint 현재 좌표
 */
void ADigitalBleedGameMode::SpawnDungeonRoom(TArray<int32> PrevPoint,TArray<int32> LastPoint, TArray<int32> NextPoint)
{
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (MapWallNoneClass) // 헤더에 TSubclassOf<AActor> MapWallNoneClass 선언 필요
	{
		FVector SpawnLocationFirstRoom = GMapRoomStartOffset + FVector(LastPoint[1] * GTileSize, LastPoint[0] * GTileSize, 0.0f);
		
		// UE_LOG(LogTemp, Log, TEXT("[%d,%d] Spawned at %s"), LastPoint[0], LastPoint[1], *SpawnLocationFirstRoom.ToString());

		ADungeonRoom* SpawnedWall = GetWorld()->SpawnActor<ADungeonRoom>(
			MapWallNoneClass,
			SpawnLocationFirstRoom,
			SpawnRotation,
			SpawnParams
		);

		if (LastPoint[0] >= 0)
		{
			Rooms[LastPoint[0]][LastPoint[1]] = SpawnedWall; 
		}
		// [LastPoint[0]][LastPoint[1]
		SpawnedWall->Direction = this->DirectionData[LastPoint[0]][LastPoint[1]];
		if (PrevPoint[0] >= 0)
		{
			SpawnedWall->PrevDirection = this->DirectionData[PrevPoint[0]][PrevPoint[1]];
		} else
		{
			SpawnedWall->PrevDirection = -1;
		}
		SpawnedWall->MapType = this->MapData[LastPoint[0]][LastPoint[1]];
		SpawnedWall->SetDirection();
		//UE_LOG(LogTemp, Log, TEXT("[%d,%d] Spawned at %s"), NextPoint[0], NextPoint[1], *SpawnLocation.ToString());
	}
}

/**
 * 시작점에서 끝점을 연결하는 확실한 길을 만든다.
 * 이건 Recursive Function 이다. 그래서 함수가 스스로를 재귀호출 하고 있음.
 * @author Junghee Wang
 */
void ADigitalBleedGameMode::GenerateCriticalPath(TArray<int32> PrevPoint, TArray<int32> LastPoint, int32 Length)
{
	// 시작점에서 진행할 방향을 정한다.
	int32 RandomDirection = FMath::RandRange(0, 3);
	TArray<int32> NextPoint = LastPoint;
	bool bIsOk = false;

	switch (RandomDirection)
	{
	case 0: //UP
		NextPoint[0] -= 1;
		break;
	case 1: // RIGHT
		NextPoint[1] += 1;
		break;
	case 2: // DOWN
		NextPoint[0] += 1;
		break;
	case 3: // LEFT
		NextPoint[1] -= 1;
		break;
	}

	if (NextPoint[0] >= 0 && NextPoint[0] < GDungeonMapWidth &&
		NextPoint[1] >= 0 && NextPoint[1] < GDungeonMapWidth &&
		this->MapData[NextPoint[0]][NextPoint[1]] == 0
	)
	{
		bIsOk = true;
	}

	if (bIsOk)
	{
		//다음 포인트는 3을 넣고, 이전 포인트는 1을 넣어서 마지막 룸이 3이 되도록 만듦.
		this->MapData[NextPoint[0]][NextPoint[1]] = 3;
		if (this->MapData[LastPoint[0]][LastPoint[1]] != 2)
		{
			this->MapData[LastPoint[0]][LastPoint[1]] = FMath::RandBool() ? 1 : 4;
		}
	
		this->DirectionData[LastPoint[0]][LastPoint[1]] = RandomDirection;

		this->SpawnDungeonRoom(PrevPoint, LastPoint, NextPoint);
	}

	if (Length == 0)
	{
		this->MapData[LastPoint[0]][LastPoint[1]] = 3;
		this->SpawnDungeonRoom(PrevPoint, LastPoint, LastPoint);
	}

	if (Length <= 0) return;

	if (!bIsOk)
	{
		this->GenerateCriticalPath(PrevPoint, LastPoint, Length-1); //다시 호출하여 Direction 랜덤값을 다시 받아오길 희망
		return;
	}

	//Length-1 해서 재귀호출
	this->GenerateCriticalPath(LastPoint, NextPoint, Length-1);
}

void ADigitalBleedGameMode::GenerateBranchPath(TArray<int32> PrevPoint, TArray<int32> LastPoint, int32 Length)
{
	
	
	int32 RandomDirection = FMath::RandRange(0, 3);
	TArray<int32> NextPoint = LastPoint;
	bool bIsOk = false;
	
	switch (RandomDirection)
	{
	case 0: //UP
		NextPoint[0] -= 1;
		break;
	case 1: // RIGHT
		NextPoint[1] += 1;
		break;
	case 2: // DOWN
		NextPoint[0] += 1;
		break;
	case 3: // LEFT
		NextPoint[1] -= 1;
		break;
	}
	
	if (NextPoint[0] >= 0 && NextPoint[0] < GDungeonMapWidth &&
		NextPoint[1] >= 0 && NextPoint[1] < GDungeonMapWidth &&
		this->MapData[NextPoint[0]][NextPoint[1]] == 0
	)
	{
		bIsOk = true;
	}
	
	
	if (bIsOk)
	{
		// 브랜치의 마지막은 6, 일반 브랜치 길은 5이다.
		this->MapData[NextPoint[0]][NextPoint[1]] = 6;
		this->MapData[LastPoint[0]][LastPoint[1]] = 5;
		this->DirectionData[LastPoint[0]][LastPoint[1]] = RandomDirection;

		
		if (PrevPoint[0] == LastPoint[0] && PrevPoint[1] == LastPoint[1])
		{
			Rooms[LastPoint[0]][LastPoint[1]]->ApplyBranch(RandomDirection);
		} else
		{
			this->SpawnDungeonRoom(PrevPoint, LastPoint, NextPoint);
		}
	}

	if (Length == 0)
	{
		this->MapData[LastPoint[0]][LastPoint[1]] = 6;
		if (PrevPoint[0] == LastPoint[0] && PrevPoint[1] == LastPoint[1])
		{
			Rooms[LastPoint[0]][LastPoint[1]]->RestoreBranch();
		} else
		{
			this->SpawnDungeonRoom(PrevPoint, LastPoint, NextPoint);
		}
	}
	

	if (Length <= 0) return;
	if (!bIsOk)
	{
		this->GenerateBranchPath(PrevPoint,LastPoint, Length-1); //다시 호출하여 Direction 랜덤값을 다시 받아오길 희망
		return;
	}
	//Length-1 해서 재귀호출
	this->GenerateBranchPath(LastPoint, NextPoint, Length-1);
}

/**
 * 맵을 2차원 배열에 생성한다.
 * @author Junghee Wang
 */
void ADigitalBleedGameMode::GenerateMap()
{
	// 시작점과 끝점을 정한다.
	this->StartPoint = {FMath::RandRange(0, GDungeonMapWidth - 1),FMath::RandRange(0, GDungeonMapWidth - 1)};
	this->CriticalPathLastPoint = this->StartPoint;
	this->MapData[this->StartPoint[0]][this->StartPoint[1]] = 2; // 시작점이면 2
}

/**
 * 후처리 function.
 * @author Junghee Wang
 */
void ADigitalBleedGameMode::PostProcess()
{
	UE_LOG(LogTemp, Log, TEXT("PostProcess"));
	for (int i = 0; i < this->MapData.Num(); i++)
	{
		for (int j = 0; j < this->MapData[i].Num(); j++)
		{
			if (this->MapData[i][j] == 2) // 만약 값이 2이면 시작점이므로
			{
				// Spawn actor MapWallNone
				FVector SpawnLocation = GMapRoomStartOffset + FVector(j * GTileSize, i * GTileSize, 100.0f);
				FRotator SpawnRotation = FRotator::ZeroRotator;
				//spawn current player at SpawnLocation
				APlayerController* PC = GetWorld()->GetFirstPlayerController();
				if (IsValid(PC))
				{
					APawn* PlayerPawn = PC->GetPawn();
					if (IsValid(PlayerPawn))
					{
						UE_LOG(LogTemp, Log, TEXT("Player Spawned at %s"), *SpawnLocation.ToString());
						PlayerPawn->SetActorLocation(SpawnLocation);
						PlayerPawn->SetActorRotation(SpawnRotation);
					}
				}
			}
		}
	}
}

void ADigitalBleedGameMode::BeginPlay()
{
	Super::BeginPlay();
	this->GenerateMap();
	this->GenerateCriticalPath({-1,-1},this->CriticalPathLastPoint, GCriticalPathLength);
	this->GenerateBranches();
	this->PrintMapInfo();

	this->PostProcess();
}

void ADigitalBleedGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
}
