// 26.02.06, Written By Junghee Wang

#include "DigitalBleedGameMode.h"

#include "CborTypes.h"
#include "HLSLTypeAliases.h"

constexpr int32 MapWidth = 10;
constexpr float TileSize = MapWidth*100.0;
const FVector StartOffset = FVector(MapWidth * -450.0f, MapWidth * -450.0f, 0.0f);
constexpr int32 CriticalPathLength = 20;
constexpr int32 BranchesMaxNum = 3;
constexpr int32 BranchLength = 4;

ADigitalBleedGameMode::ADigitalBleedGameMode()
{
	// stub
}

void ADigitalBleedGameMode::PrintMapInfo()
{
	UE_LOG(LogTemp, Log, TEXT("MAP======================"));
	for (int i = 0; i < MapWidth; i++)
	{
		FString Data = "";
		for (int j = 0; j < MapWidth; j++)
		{
			Data += FString::FromInt(this->MapData[i][j]) + " ";
		}
		UE_LOG(LogTemp, Log, TEXT("%s"), *Data);
	}
	UE_LOG(LogTemp, Log, TEXT("DIR======================"));
	for (int i = 0; i < MapWidth; i++)
	{
		FString Data = "";
		for (int j = 0; j < MapWidth; j++)
		{
			Data += FString::FromInt(this->DirectionData[i][j]) + " ";
		}
		UE_LOG(LogTemp, Log, TEXT("%s"), *Data);
	}
}

void ADigitalBleedGameMode::GenerateBranches()
{
	for (int i = 0; i < MapWidth; i ++)
	{
		for (int j = 0; j < MapWidth; j++)
		{
			if (this->MapData[i][j] == 4)
			{
				this->GenerateBranchPath({i,j}, FMath::RandRange(1, BranchLength));
			}
		}
	}
}

/**
 * 시작점에서 끝점을 연결하는 확실한 길을 만든다.
 * 이건 Recursive Function 이다. 그래서 함수가 스스로를 재귀호출 하고 있음.
 * @author Junghee Wang
 */
void ADigitalBleedGameMode::GenerateCriticalPath(TArray<int32> LastPoint, int32 Length)
{
	if (Length <= 0) return;
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

	if (NextPoint[0] >= 0 && NextPoint[0] < MapWidth &&
		NextPoint[1] >= 0 && NextPoint[1] < MapWidth &&
		this->MapData[NextPoint[0]][NextPoint[1]] == 0
	)
	{
		bIsOk = true;
	}

	if (!bIsOk)
	{
		this->GenerateCriticalPath(LastPoint, Length-1); //다시 호출하여 Direction 랜덤값을 다시 받아오길 희망
		return;
	}

	//마지막으로 Length가 1이면 3을 넣어서 Endpoint임을 표기하고 아니면 1을 넣어서 일반 길을 만듦
	this->MapData[NextPoint[0]][NextPoint[1]] = Length==1 ? 3 : 1;
	this->DirectionData[LastPoint[0]][LastPoint[1]] = RandomDirection;

	if (Length > 1)
	{
		if (FMath::RandBool() == true && NumBranches < BranchesMaxNum)
		{
			// 브랜치 후보는 4로 표기한다!
			this->MapData[NextPoint[0]][NextPoint[1]] = 4;
		}
	}

	//Length-1 해서 재귀호출
	this->GenerateCriticalPath(NextPoint, Length-1);
}

void ADigitalBleedGameMode::GenerateBranchPath(TArray<int32> LastPoint, int32 Length)
{
	if (Length <= 0) return;
	
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

	if (NextPoint[0] >= 0 && NextPoint[0] < MapWidth &&
		NextPoint[1] >= 0 && NextPoint[1] < MapWidth &&
		this->MapData[NextPoint[0]][NextPoint[1]] == 0
	)
	{
		bIsOk = true;
	}

	if (!bIsOk)
	{
		this->GenerateCriticalPath(LastPoint, Length-1); //다시 호출하여 Direction 랜덤값을 다시 받아오길 희망
		return;
	}

	// 브랜치의 마지막은 6, 일반 브랜치 길은 5이다.
	this->MapData[NextPoint[0]][NextPoint[1]] = Length==1 ? 6 : 5;
	this->DirectionData[LastPoint[0]][LastPoint[1]] = RandomDirection;

	//Length-1 해서 재귀호출
	this->GenerateBranchPath(NextPoint, Length-1);
}

/**
 * 맵을 2차원 배열에 생성한다.
 * @author Junghee Wang
 */
void ADigitalBleedGameMode::GenerateMap()
{
	// 시작점과 끝점을 정한다.
	this->StartPoint = {FMath::RandRange(0, MapWidth - 1),FMath::RandRange(0, MapWidth - 1)};
	this->CriticalPathLastPoint = this->StartPoint;
	this->MapData[this->StartPoint[0]][this->StartPoint[1]] = 2; // 시작점이면 2
}

/**
 * 생성된 2차원 배열 맵을 3D 공간에 스폰시킨다.
 * @author Junghee Wang
 */
void ADigitalBleedGameMode::SpawnMap()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	for (int i = 0; i < this->MapData.Num(); i++)
	{
		for (int j = 0; j < this->MapData[i].Num(); j++)
		{
			if (this->MapData[i][j] >= 1)
			{
				// Spawn actor MapWallNone
				FVector SpawnLocation = StartOffset + FVector(j * TileSize, i * TileSize, 0.0f);
				FRotator SpawnRotation = FRotator::ZeroRotator;

				if (this->MapData[i][j] == 2) // 만약 값이 2이면 시작점이므로
				{
					//spawn current player at SpawnLocation
					APlayerController* PC = GetWorld()->GetFirstPlayerController();
					if (IsValid(PC))
					{
						APawn* PlayerPawn = PC->GetPawn();
						if (IsValid(PlayerPawn))
						{
							PlayerPawn->SetActorLocation(SpawnLocation + FVector(0.0f, 0.0f, 100.0f));
							PlayerPawn->SetActorRotation(SpawnRotation);
						}
					}
				}

				if (MapWallNoneClass) // 헤더에 TSubclassOf<AActor> MapWallNoneClass 선언 필요
				{
					AActor* SpawnedWall = GetWorld()->SpawnActor<AActor>(
						MapWallNoneClass,
						SpawnLocation,
						SpawnRotation,
						SpawnParams
					);

					if (IsValid(SpawnedWall))
					{
						UE_LOG(LogTemp, Log, TEXT("Spawned wall at [%d,%d]: %s"), 
						       i, j, *SpawnLocation.ToString());
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
	this->GenerateCriticalPath(this->CriticalPathLastPoint, CriticalPathLength);
	this->GenerateBranches();
	this->SpawnMap();
	this->PrintMapInfo();
}
