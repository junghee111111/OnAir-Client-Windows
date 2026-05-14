// 2026.01.13 재작성 시작, Written By Junghee Wang
// 2026.02.02 리팩토링 1차
// 2026.05.14 리팩토링 2차

#include "BattleGameMode.h"

#include "BattleGamePlayerState.h"
#include "Battle/LifeHuman.h"
#include "Battle/Camera/BattleMainCam.h"
#include "GameFramework/PlayerStart.h"
#include "NiagaraFunctionLibrary.h"
#include "Battle/Misc/DamageTypeWeak.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/DigitalBleedGameInstance.h"
#include "UI/Battle/WholeBattleUI.h"

namespace BattleConstants
{
	constexpr float ENEMY_SPAWN_RADIUS = 200.0f; // 적들을 원형으로 중앙에 배치하는데 이것의 radius이다.
	constexpr float TIMER_CALCULATE_ORDER = 0.5f;
	constexpr float TIMER_INIT_UI = 3.0f;
	constexpr float TIMER_START_ION = 4.0f;
	constexpr float TIMER_END_TURN = 1.0f;
	constexpr float TIMER_ITEM_EXECUTION = 2.0f;
	constexpr float TIMER_SKILL_EXECUTION = 1.0f;
	constexpr float CAMERA_ADJUSTMENT_DELAY = 3.0f;
	constexpr float WEAK_DAMAGE_MULTIPLIER = 1.2f;
	constexpr float IMMUNE_DAMAGE_MULTIPLIER = 0.5f;
	constexpr float DEFENSE_MULTIPLIER = 1.5f;
	constexpr int32 ION_TIMER_BASE = 2; // 기본적으로 몇초에 한번씩 이온을 떨어뜨릴것인가?
	constexpr float ION_TIMER_DIVISOR = 50.0f; // 이온 타이머 관련 나눗셈 상수
}

ABattleGameMode::ABattleGameMode()
{
	PlayerStateClass = ABattleGamePlayerState::StaticClass();
	CurrentPlayerIndex = 0;
}

void ABattleGameMode::SpawnEnemies()
{
	if (this->LifeEnemies.Num() == 0) return;
	if (this->LifeEnemies.Num() == 1)
	{
		// Spawn at 0,0,0
		FVector SpawnLocation = FVector(0, 0, 0);
		FRotator SpawnRotation = FRotator(0, 0, 0);
		ALifeEnemy* Enemy = GetWorld()->SpawnActor<ALifeEnemy>(this->LifeEnemies[0], SpawnLocation, SpawnRotation);
		Enemy->SetTmpCode("Enemy_0");
		Enemies.Add(Enemy);
	}
	if (this->LifeEnemies.Num() > 1)
	{
		// Spawn in circle shape with same degrees in 360
		float AngleBetweenEnemies = PI*2 / this->LifeEnemies.Num();
		float distance = BattleConstants::ENEMY_SPAWN_RADIUS;
		for (int i = 0; i < this->LifeEnemies.Num(); i++)
		{
			float Angle = AngleBetweenEnemies * i + PI/8; // 45도 비틀어서 추가하기 위해 PI/8을 추가한다.
			FVector SpawnLocation = FVector(distance * FMath::Cos(Angle), distance * FMath::Sin(Angle), 0);

			FRotator LookRotation = (SpawnLocation-FVector(0,0,0)).Rotation();
			FRotator SpawnRotation = FRotator(0, LookRotation.Yaw, 0);
			ALifeEnemy* Enemy = GetWorld()->SpawnActor<ALifeEnemy>(this->LifeEnemies[i], SpawnLocation, SpawnRotation);
			Enemy->SetTmpCode(FString::Printf(TEXT("Enemy_%d"),i));
			Enemies.Add(Enemy);
		}
	}
}


void ABattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

AActor* ABattleGameMode::GetPlayerStartByIndex(int32 PlayerIndex) const
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
    
	if (PlayerStarts.IsValidIndex(PlayerIndex))
	{
		return PlayerStarts[PlayerIndex];
	}
    
	return nullptr;
}


void ABattleGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	APlayerController* PC = Cast<APlayerController>(NewPlayer);

	if (!IsValid(PC) || !PlayerControllers.Contains(PC)) return; 

	int32 PlayerIndex = PlayerControllers.Find(PC);
	AActor* StartSpot = this->GetPlayerStartByIndex(PlayerIndex);
	
	if (StartSpot != nullptr && IsValid(StartSpot))
	{
		// 스폰 위치 및 회전값
		FVector SpawnLocation = StartSpot->GetActorLocation();
		PC->GetPawn()->SetActorLocation(SpawnLocation);
	}
}

void ABattleGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ABattleGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

/**
 * This is legacy code!!!
 */
void ABattleGameMode::StartIonTimer()
{
	for (ALife* Life : PartyMembers)
	{
		FTimerHandle TimerHandle;
		float TimerDuration = Life->LifeStatComponent->GetCon()/BattleConstants::ION_TIMER_DIVISOR
			+Life->LifeStatComponent->GetLevel()/BattleConstants::ION_TIMER_DIVISOR
			+ BattleConstants::ION_TIMER_BASE;
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this,Life]()
		{
			if (this->bEndGame) return;
			Life->LifeStatComponent->DecreaseIons();
			this->DispatcherLifeHit.Broadcast();
		}, TimerDuration, true);
		this->IonTimerHandles.Add(TimerHandle);
	}
}

void ABattleGameMode::AdjustCam()
{
	ALife* CurrentTurnLife = AccessLifeByCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBack(CurrentTurnLife->GetActorLocation());
		this->MainCam->StopRotation();
		CurrentTurnLife->ResetAnim();
	}
}

void ABattleGameMode::SpawnPlayers()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	UDigitalBleedGameInstance* GI = Cast<UDigitalBleedGameInstance>(GetGameInstance());

	
	
	for (int32 i = 1; i < GI->GetPartyLength(); ++i) // 나를 제외하기 위해 i를 1부터 시작시킴
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		APlayerController* PC = GetWorld()->SpawnActor<APlayerController>(
			APlayerController::StaticClass(),
			PlayerStarts[i]->GetActorLocation(),
			PlayerStarts[i]->GetActorRotation(),
			SpawnParams);

		if (IsValid(PC))
		{
			AActor* StartSpot = this->GetPlayerStartByIndex(i);

			if (StartSpot == nullptr) continue;
			
			this->RegisterPlayerController(PC);
			FTransform SpawnTransform = StartSpot->GetActorTransform();

			FString PlayerCode = GI->GetPartyMembers()[i];
			AActor* SpawnedCharacter = nullptr;

			if (PlayerCode.Equals(FString("HYJ"), ESearchCase::IgnoreCase))
			{
				SpawnedCharacter= GetWorld()->SpawnActor(Player_HYJClass, &SpawnTransform, SpawnParams);
			}else if (PlayerCode.Equals(FString("CJY"), ESearchCase::IgnoreCase))
			{
				SpawnedCharacter= GetWorld()->SpawnActor(Player_CJYClass, &SpawnTransform, SpawnParams);
			} else if (PlayerCode.Equals(FString("JAR"), ESearchCase::IgnoreCase))
			{
				SpawnedCharacter= GetWorld()->SpawnActor(Player_JARClass, &SpawnTransform, SpawnParams);
			} else
			{
				SpawnedCharacter = GetWorld()->SpawnActor(DefaultPawnClass, &SpawnTransform, SpawnParams);
			}
			
			
			ALifeHuman* LifeHuman = Cast<ALifeHuman>(SpawnedCharacter);
			if (IsValid(LifeHuman))
			{
				// PC->Possess(LifeHuman);
				LifeHuman->PlayerCode = GI->GetPartyMembers()[i];
				LifeHuman->SetTmpCode(GI->GetPartyMembers()[i]);
				PartyMembers.Add(LifeHuman);
			}

			UE_LOG(
			LogTemp, Warning,
			TEXT("BattleGameMode :: SpawnPlayer (%s) at Pos : %d, Num : %d"),
			*GI->GetPartyMembers()[i], i, PlayerControllers.Num());
		}
	}
}

void ABattleGameMode::RegisterPlayerController(APlayerController* PC)
{
	if (IsValid(PC))
	{
		if (PlayerControllers.Contains(PC)) return;
		PlayerControllers.Add(PC);
		UE_LOG(
		LogTemp, Warning,
		TEXT("BattleGameMode :: Player Controller Registered : %s, Num : %d"),
		*PC->GetName(), PlayerControllers.Num());
	}
}

void ABattleGameMode::InitializeMainCamera()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Error, TEXT("BattleGameMode :: No PlayerController found"));
		return;
	}
	
	TArray<AActor*> FoundCams;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleMainCam::StaticClass(), FoundCams);

	for (AActor* Actor : FoundCams)
	{
		ABattleMainCam* BattleMainCam = Cast<ABattleMainCam>(Actor);
		check(BattleMainCam);
		if (Actor->ActorHasTag("MainCam"))
		{
			PC->SetViewTargetWithBlend(
				Actor,
				0.f,
				VTBlend_EaseInOut,
				0.f
			);
			BattleMainCam->SetPitch(-10.0f);
			BattleMainCam->StartRotation();
			//UE_LOG(LogTemp, Warning, TEXT("BattleGameMode :: MainCam Initialized!"));
			this->MainCam = BattleMainCam;
			return;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("BattleGameMode :: No MainCam found"));
}

void ABattleGameMode::InitializeUI()
{
	if (IsValid(this->WholeBattleUIClass))
	{
		this->WholeBattleUI_Instance = CreateWidget<UWholeBattleUI>(GetWorld()->GetFirstPlayerController(), this->WholeBattleUIClass);
		
		if (IsValid(this->WholeBattleUI_Instance))
		{
			this->WholeBattleUI_Instance->AddToViewport();
		}
	}
}

void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllers.Empty();
	CurrentPlayerIndex = 0;
	this->SpawnEnemies();
	
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	this->MyGameInstance = Cast<UDigitalBleedGameInstance>(GetGameInstance());
	
	if (!IsValid(this->MyGameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("BattleGameMode :: GameInstance Initialize Failed!"));
		return;
	}

	if (this->MyGameInstance->GetPartyLength() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BattleGameMode :: There's no party member!"));
		return;
	}

	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();
	// 첫 번째 플레이어(나) 추가
	if (IsValid(FirstPC))
	{
		FirstPC->SetInputMode(FInputModeGameOnly());
		FirstPC->bShowMouseCursor = true;
		
		ALifeHuman* MyLifeHuman = Cast<ALifeHuman>(FirstPC->GetPawn());
		if (IsValid(MyLifeHuman))
		{
			MyLifeHuman->PlayerCode = this->MyGameInstance->GetPartyMembers()[0]; // 첫 번째 PlayerCode 설정
			MyLifeHuman->SetTmpCode(this->MyGameInstance->GetPartyMembers()[0]);
			PartyMembers.Add(MyLifeHuman);
			MyLifeHuman->SetActorLocationAndRotation(PlayerStarts[0]->GetActorLocation(),PlayerStarts[0]->GetActorRotation());
			// FirstPC->Possess(MyLifeHuman);
		}
		RegisterPlayerController(FirstPC);
		SpawnPlayers();
	}

	if (PlayerControllers.Num() > 0)
	{
		this->InitializeMainCamera();
	}
	

	FTimerHandle TimerHandleCalculateOrder;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleCalculateOrder, [this]()
	{
		CalculatePartyOrder();
		CurrentTurnTarget = PartyOrder[0];
	}, BattleConstants::TIMER_CALCULATE_ORDER, false);
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		this->InitializeUI();
		this->InitNewTurn();
	}, BattleConstants::TIMER_INIT_UI, false);

	FTimerHandle TimerHandle3;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle3, [this]()
	{
		this->AdjustCam();
	}, BattleConstants::TIMER_START_ION, false);
}

void ABattleGameMode::CalculatePartyOrder()
{
	// order party member as Spd desc order
	PartyOrder.Empty();
	
	// 유효하지 않은 요소 필터링
	TArray<ALifeHuman*> SortedMembers;
	for (ALifeHuman* Member : PartyMembers)
	{
		if (IsValid(Member) && IsValid(Member->LifeStatComponent))
		{
			SortedMembers.Add(Member);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid PartyMember or LifeStatComponent found!"));
		}
	}
	
	TArray<ALifeEnemy*> SortedEnemies;
	for (ALifeEnemy* Enemy : Enemies)
	{
		if (IsValid(Enemy) && IsValid(Enemy->LifeStatComponent))
		{
			SortedEnemies.Add(Enemy);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid Enemy or LifeStatComponent found!"));
		}
	}
	
	TArray<ALife*> SortedAll = {};

	UE_LOG(LogTemp, Log, TEXT("Sort Members"));
	SortedMembers.Sort([](const ALifeHuman& A, const ALifeHuman& B)
	{
		// 이 시점에서는 이미 필터링되어 안전함
		return A.LifeStatComponent->GetSpd() > B.LifeStatComponent->GetSpd();
	});

	UE_LOG(LogTemp, Log, TEXT("Sort Enemy"));
	SortedEnemies.Sort([](const ALifeEnemy& A, const ALifeEnemy& B)
	{
		//UE_LOG(LogTemp, Log, TEXT("%s : %s"), *A.GetName(), *B.GetName());
		return A.LifeStatComponent->GetSpd() > B.LifeStatComponent->GetSpd();
	});

	SortedAll.Append(SortedMembers);
	SortedAll.Append(SortedEnemies);

	UE_LOG(LogTemp, Log, TEXT("Sort All"));
	SortedAll.Sort([](const ALife& A, const ALife& B)
	{
		return A.LifeStatComponent->GetSpd() > B.LifeStatComponent->GetSpd();
	});

	UE_LOG(LogTemp, Warning, TEXT("SortedAll.Num() : %d"), SortedAll.Num());

	int32 MobIdx = 0;
	for (int i = 0; i < SortedAll.Num(); i++)
	{
		ALifeHuman* LifeHuman = Cast<ALifeHuman>(SortedAll[i]);
		if (IsValid(LifeHuman))
		{
			PartyOrder.Add(LifeHuman->PlayerCode);
		} else
		{
			FString TmpMobCode = FString::Printf(TEXT("Enemy_%d"), MobIdx);
			PartyOrder.Add(TmpMobCode);
			MobIdx++;
		}
	}

	for (int i = 0; i < PartyOrder.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("PartyOrder[%d] : %s"), i, *PartyOrder[i]);
	}
}

void ABattleGameMode::CTScan()
{
	if (bIsPlayerSideTurn==false) return;
	bCTScanMode = true;
	StartSelectEnemyMode();
}

void ABattleGameMode::CameraSeeEnemyOnly(int32 EnemyIdx)
{
	if(IsValid(Enemies[EnemyIdx]))
	{
		this->MainCam->SeePlayerBack(Enemies[EnemyIdx]->GetActorLocation());
	}
}

void ABattleGameMode::CameraSeeTurnOwner()
{
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBack(CurrentTurnLife->GetActorLocation());
	}
}

void ABattleGameMode::CameraSee_SkillTargets_All()
{
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBackToEnemy(CurrentTurnLife->GetActorLocation(), FVector::Zero());
	}
}

void ABattleGameMode::CameraSee_SkillTarget_Angle1()
{
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBackToEnemy_Angle1(CurrentTurnLife->GetActorLocation(), this->CurrentSkillTargets[0]->GetActorLocation());
	}
}

void ABattleGameMode::CameraSee_SkillTarget_Angle2()
{
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBackToEnemy_Angle2(CurrentTurnLife->GetActorLocation(), this->CurrentSkillTargets[0]->GetActorLocation());
	}
}

void ABattleGameMode::CameraSee_SkillTarget_Angle3()
{
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBackToEnemy_Angle3(CurrentTurnLife->GetActorLocation(), this->CurrentSkillTargets[0]->GetActorLocation());
	}
}

void ABattleGameMode::CameraSee_SkillTarget_Angle4()
{
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBackToEnemy_Angle3(this->CurrentSkillTargets[0]->GetActorLocation(),CurrentTurnLife->GetActorLocation());
	}
}

void ABattleGameMode::CameraSee_SkillTarget_FromEnemy()
{
	ALife* CurrentTurnLife = AccessLifeByCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeeEnemyBackToPlayer(this->CurrentSkillTargets[0]->GetActorLocation(), CurrentTurnLife->GetActorLocation());
	}
}

void ABattleGameMode::CameraSee_SkillTarget_EveryBody()
{
	ALife* CurrentTurnLife = AccessLifeByCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->StartRotationWithArmLength(700.0f);
	}
}

void ABattleGameMode::StartSelectEnemyMode()
{
	this->bEnemySelectMode = true;
	UDigitalBleedGameInstance* GI = Cast<UDigitalBleedGameInstance>(GetGameInstance());
	
	if (IsValid(GI))
	{
		GI->ShowToast(FText::FromString("BATTLE_SELECT_TARGET"));
		
		ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
		ALifeEnemy* SelectedEnemyLife = Enemies[SelectedEnemy];
		this->CurrentSkillTargets = {SelectedEnemyLife};

		if (IsValid(SelectedEnemyLife))
		{
			this->MainCam->SeePlayerBackToEnemy(CurrentLife->GetActorLocation(), SelectedEnemyLife->GetActorLocation());

			if (this->LockOnIndicator != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Destroy LockOnIndicator"));
				this->LockOnIndicator->Destroy();
			}

			for (AActor* Indicator : this->LockOnIndicators)
			{
				Indicator->Destroy();
			}
			
			this->LockOnIndicator = GetWorld()->SpawnActor(LockOnIndicatorClass);
			if (IsValid(this->LockOnIndicator)) this->LockOnIndicator->SetActorLocation(SelectedEnemyLife->GetActorLocation());
			
			for (ALifeEnemy* Enemy : Enemies)
			{
				if (IsValid(Enemy)) Enemy->ShowHpBar();
			}
		} else
		{
			UE_LOG(LogTemp,Error,TEXT("[BattleGameMode::StartSelectEnemyMode] Invalid SelectedEnemyLife"))
		}
	}
}

void ABattleGameMode::StartSelectAllEnemy()
{
	this->bEnemySelectMode = true;
	UDigitalBleedGameInstance* GI = Cast<UDigitalBleedGameInstance>(GetGameInstance());
	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();
	if (IsValid(GI))
	{
		GI->ShowToast(FText::FromString("BATTLE_ALL_SELECTED"));
		UE_LOG(LogTemp, Warning, TEXT("StartSelectAllEnemy :: Try to select all enemy!"));
		
		ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
		this->MainCam->SeePlayerBackToEnemy(CurrentLife->GetActorLocation(), FVector::Zero());

		if (this->LockOnIndicator != nullptr)
		{
			this->LockOnIndicator->Destroy();
		}
		
		for (AActor* Indicator : this->LockOnIndicators)
		{
			Indicator->Destroy();
		}
		
		this->LockOnIndicators.Empty();
		this->CurrentSkillTargets.Empty();
		
		for (ALifeEnemy* Enemy : Enemies)
		{
			if (Enemy->LifeStatComponent->GetHp() <= 0)
			{
				UE_LOG(LogTemp, Error, TEXT("  ==> Skip enemy due to dead"));
				continue;
			};
			if (IsValid(Enemy)) Enemy->ShowHpBar();
			
			AActor* Indicator = GetWorld()->SpawnActor(LockOnIndicatorClass);
			if (IsValid(Indicator)) Indicator->SetActorLocation(Enemy->GetActorLocation());
			
			this->LockOnIndicators.Add(Indicator);
			this->CurrentSkillTargets.Add(Enemy);
			UE_LOG(LogTemp, Warning, TEXT("  => %s added."), *Enemy->GetName());
		}
	}
}

void ABattleGameMode::SelectNextEnemy()
{
	this->SelectedEnemy++;
	if (SelectedEnemy >= LifeEnemies.Num()) SelectedEnemy = GetFirstAliveEnemy();
	
	FString NextEnemyTmpCode = FString::Printf(TEXT("Enemy_%d"),SelectedEnemy);
	
	if (DeadList.Contains(NextEnemyTmpCode))
	{
		UE_LOG(LogTemp, Log, TEXT("%s -- Canceled"), *NextEnemyTmpCode);
		this->SelectNextEnemy();
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *NextEnemyTmpCode);
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	ALifeEnemy* SelectedEnemyLife = Enemies[SelectedEnemy];
	if (!IsValid(SelectedEnemyLife)) return;
	this->MainCam->SeePlayerBackToEnemy(CurrentLife->GetActorLocation(), SelectedEnemyLife->GetActorLocation());
	this->CurrentSkillTargets = {SelectedEnemyLife};

	if (IsValid(this->LockOnIndicator)) this->LockOnIndicator->SetActorLocation(SelectedEnemyLife->GetActorLocation());
}

void ABattleGameMode::SelectPrevEnemy()
{
	this->SelectedEnemy--;
	if (SelectedEnemy < 0) SelectedEnemy = GetLastAliveEnemy();
	
	FString PrevEnemyTmpCode = FString::Printf(TEXT("Enemy_%d"),SelectedEnemy);
	
	if (DeadList.Contains(PrevEnemyTmpCode))
	{
		UE_LOG(LogTemp, Log, TEXT("%s -- Canceled"), *PrevEnemyTmpCode);
		this->SelectPrevEnemy();
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *PrevEnemyTmpCode);
	
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	ALifeEnemy* SelectedEnemyLife = Enemies[SelectedEnemy];
	if (!IsValid(SelectedEnemyLife)) return;
	this->MainCam->SeePlayerBackToEnemy(CurrentLife->GetActorLocation(), SelectedEnemyLife->GetActorLocation());
	this->CurrentSkillTargets = {SelectedEnemyLife};

	if (IsValid(this->LockOnIndicator)) this->LockOnIndicator->SetActorLocation(SelectedEnemyLife->GetActorLocation());
}

int32 ABattleGameMode::GetFirstAliveEnemy()
{
	for (int32 i = 0; i<PartyOrder.Num(); i++)
	{
		if (PartyOrder[i].StartsWith("Enemy_"))
		{
			ALife* Enemy = this->AccessLifeByCode(PartyOrder[i]);
			if (IsValid(Enemy))
			{
				if (DeadList.Contains(PartyOrder[i]) == false)
				{
					return GetEnemyIdxFromEnemyCode(PartyOrder[i]);
				}
			} 
		}
	}
	return -1;
}

int32 ABattleGameMode::GetLastAliveEnemy()
{
	int32 Order = -1;
	for (int32 i = 0; i<PartyOrder.Num(); i++)
	{
		if (PartyOrder[i].StartsWith("Enemy_"))
		{
			ALife* Enemy = this->AccessLifeByCode(PartyOrder[i]);
			if (IsValid(Enemy))
			{
				if (DeadList.Contains(PartyOrder[i]) == false)
				{
					if (Order < GetEnemyIdxFromEnemyCode(PartyOrder[i]))
					{
						Order = GetEnemyIdxFromEnemyCode(PartyOrder[i]);
					}
				}
			} 
		}
	}
	return Order;
}

int32 ABattleGameMode::GetEnemyIdxFromEnemyCode(FString EnemyCode)
{
	TArray<FString> Parts;
	EnemyCode.ParseIntoArray(Parts, TEXT("_"));

	if (Parts.Num() < 2) return -1;
	return FCString::Atoi(*Parts[1]);
}

void ABattleGameMode::ApplyItemEffect()
{
	if (this->CurrentItem.Name.IsEmpty()) return;
	if (this->CurrentItemTargets.Num() > 0)
	{
		for (ALife* Life : this->CurrentItemTargets)
		{
			Life->LifeStatComponent->ManipulateHp(this->CurrentItem.HpInc);
			Life->LifeStatComponent->ManipulateHb(this->CurrentItem.HbInc);
			Life->LifeStatComponent->ManipulateSodium(this->CurrentItem.NaInc);
			Life->LifeStatComponent->ManipulatePotassium(this->CurrentItem.KInc);
		}
	}
}

void ABattleGameMode::EndSelectEnemyMode()
{
	this->bEnemySelectMode = false;
	if (this->bCTScanMode) this->bCTScanMode = false;
	
	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();
	FirstPC->bShowMouseCursor = true;
	
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	for (ALifeEnemy* Enemy : Enemies)
	{
		if (IsValid(Enemy)) Enemy->HideHpBar();
	}
	if (IsValid(this->LockOnIndicator))
	{
		this->LockOnIndicator->Destroy();
	}

	if (this->LockOnIndicators.Num() > 0)
	{
		for (AActor* Indicator : this->LockOnIndicators)
		{
			Indicator->Destroy();
		}
	}
	this->LockOnIndicators.Empty();
	this->LockOnIndicator = nullptr;
	
	this->MainCam->SeePlayerBack(CurrentLife->GetActorLocation());
}

void ABattleGameMode::StartSelectPlayerMode()
{
	this->bPlayerSelectMode = true;
	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();
	FirstPC->bShowMouseCursor = false;
	
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	
	for (ALifeHuman* Member : PartyMembers)
	{
		if (IsValid(Member)) Member->ShowHpBar();
	}
	this->MainCam->SeePlayerCenterToMargin(CurrentLife->GetActorLocation());
	this->CurrentItemTargets = {CurrentLife};
}

void ABattleGameMode::SelectNextPlayer()
{
	ALifeHuman* CurrentLife = Cast<ALifeHuman>(this->CurrentItemTargets[0]);
	int32 CurrentIdx = PartyMembers.Find(CurrentLife);
	
	if (CurrentIdx == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentTurnTarget not found in PartyOrder"));
		return;
	}
	
	int32 NextPlayerIdx = CurrentIdx + 1;
	if (NextPlayerIdx >= PartyMembers.Num()) 
	{
		NextPlayerIdx = 0;
	}
	
	this->CurrentItemTargets = {AccessLifeByCode(PartyMembers[NextPlayerIdx]->PlayerCode)};
	
	if (IsValid(this->MainCam) && IsValid(this->CurrentItemTargets[0]))
	{
		this->MainCam->SeePlayerCenterToMargin(this->CurrentItemTargets[0]->GetActorLocation());
	}
}

void ABattleGameMode::SelectPrevPlayer()
{
	ALifeHuman* CurrentLife = Cast<ALifeHuman>(this->CurrentItemTargets[0]);
	int32 CurrentIdx = PartyMembers.Find(CurrentLife);
	
	if (CurrentIdx == INDEX_NONE)
	{
		// CurrentTurnTarget이 PartyOrder에 없는 경우
		UE_LOG(LogTemp, Warning, TEXT("CurrentTurnTarget not found in PartyOrder"));
		return;
	}
	
	int32 PrevPlayerIdx = CurrentIdx - 1;
	if (PrevPlayerIdx < 0) 
	{
		PrevPlayerIdx = PartyMembers.Num() - 1;
	}
	
	this->CurrentItemTargets = {AccessLifeByCode(PartyMembers[PrevPlayerIdx]->PlayerCode)};
	
	if (IsValid(this->MainCam) && IsValid(this->CurrentItemTargets[0]))
	{
		this->MainCam->SeePlayerCenterToMargin(this->CurrentItemTargets[0]->GetActorLocation());
	}
}

void ABattleGameMode::EndSelectPlayerMode()
{
	this->bPlayerSelectMode = false;
	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();
	FirstPC->bShowMouseCursor = true;
	
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	for (ALifeHuman* Member : PartyMembers)
	{
		if (IsValid(Member)) Member->HideHpBar();
	}
	this->MainCam->SeePlayerBack(CurrentLife->GetActorLocation());
}

void ABattleGameMode::ExecuteItem()
{
	if (this->CurrentItemTargets.Num() > 0)
	{
		if (this->CurrentItemTargets.Num() == 1 && this->CurrentItemTargets[0]->LifeStatComponent->GetHp() <= 0)
		{
			MyGameInstance->ShowToast(FText::FromString("BATTLE_ALREADY_DEAD"));
			return;
		}
	}
	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();
	
	this->bIsItemPlaying = true;
	this->EndSelectEnemyMode();
	this->EndSelectPlayerMode();

	FString ItemNameKey = FString::Printf(TEXT("%s_NAME"), *this->CurrentItemRecord.ItemId.ToString());
	this->MyGameInstance->ShowToastItemName(FText::FromString(ItemNameKey));

	if (IsValid(this->CurrentItemTargets[0]))
	{
		this->MainCam->SeePlayerCenterToMargin(this->CurrentItemTargets.Num() == 1 ? this->CurrentItemTargets[0]->GetActorLocation() : FVector::Zero());
		this->MainCam->StartRotationWithArmLength(200.0f);
		this->CurrentItemTargets[0]->ShowHpBar();


		
		if (this->CurrentItem.PreEffect)
		{
			for (ALife* Target : this->CurrentItemTargets)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					this->CurrentItem.PreEffect,
					Target->GetActorLocation() - FVector(0.0f, 0.0f, 80.0f),
					FRotator::ZeroRotator,
					FVector(1.0f),
					true,
					true,
					ENCPoolMethod::None,
					true
				);
			}
			
			this->MyGameInstance->PlaySFX(this->SFX_ItemUse);
		}

		this->ApplyItemEffect();

		FTimerHandle Th;
		GetWorldTimerManager().SetTimer(Th, [this]()
		{
			this->bIsItemPlaying = false;
			this->bItemSelectedMode = false;
			this->EndTurn();
		},2.0f, false);
	}
}

void ABattleGameMode::ExecuteSkill()
{
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	bool IsPlayerTurn = !this->CurrentTurnTarget.StartsWith("Enemy_");
	
	if (CurrentLife->LifeStatComponent->GetHp() <= this->CurrentSkill.CostHP)
	{
		this->MyGameInstance->ShowToast(FText::FromString("BATTLE_HP_NOT_ENOUGH"));
		return;
	}
	
	//스킬네임 토스트로 표출
	this->bIsSkillPlaying = true;
	this->EndSelectEnemyMode();
	
	FString SkillNameKey = FString::Printf(TEXT("%s_TITLE"), *this->CurrentSkillRecord.SkillId.ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *this->CurrentSkillRecord.SkillId.ToString(), *SkillNameKey);
	this->MyGameInstance->ShowToastSkillName(FText::FromString(SkillNameKey));
	

	// 만약 플레이어가 스킬을 시전하면 면상을 한번 보여주게함
	// 마법계 스킬일때만 손목을 그으므로 물리계스킬일때는 얼굴 안보여줘도 댐!
	if (!this->CurrentTurnTarget.StartsWith("Enemy_") && this->CurrentSkill.Elemental > 0)
	{
		this->MainCam->SeePlayerCenterToMargin(CurrentLife->GetActorLocation());
	}

	FTimerHandle Th;
	GetWorldTimerManager().SetTimer(Th, [this,IsPlayerTurn]()
	{
		if (this->CurrentSkill.Name.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("CurrentSkill is not valid"));
			this->EndTurn();
			return;
		}
		
		ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
		
		if (this->CurrentSkill.bIsAllAttack == false)
		{
			if (this->CurrentSkill.Elemental == 0 || this->CurrentSkill.Elemental >0 && IsPlayerTurn == false)
			{
				this->MainCam->GoTowardsTarget(CurrentLife->GetActorLocation(), this->CurrentSkillTargets[0]->GetActorLocation());
			}
		}
		
		CurrentLife->ExecSkill(this->CurrentSkillTargets, this->CurrentSkill, this->CurrentSkillRecord);
		CurrentLife->ShowHpBar();

		for (ALife* Target : this->CurrentSkillTargets)
		{
			Target->ShowHpBar();
		}
		
	},1.0f, false);
}

TArray<ALife*> ABattleGameMode::GetAllAliveEnemies()
{
	TArray<ALife*> Tmp = {};
	// 리워드 경험치 어플라이
	for (ALifeEnemy* Enemy : Enemies)
	{
		if (Enemy->LifeStatComponent->GetHp() > 0) // 살아있는 멤버만..
		{
			Tmp.Add(Enemy);
		}
	}

	return Tmp;
}

void ABattleGameMode::EndSkill()
{
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);

	for (ALife* Target : this->CurrentSkillTargets)
	{
		if (IsValid(Target)) Target->HideHpBar();
	}
	
	CurrentLife->HideHpBar();
	
	this->bIsSkillPlaying = false;

	if (NumDeadEnemies == Enemies.Num())
	{
		this->EndGame();
		return;
	}
	
	if (bIsCurrentTurnOneMore == false)
	{
		this->EndTurn();
	} else
	{
		this->CameraSeeTurnOwner();
		if (this->bIsPlayerSideTurn==false)
		{
			ALifeEnemy* CurrentEnemy = Cast<ALifeEnemy>(AccessLifeByCode(CurrentTurnTarget));
			if (IsValid(CurrentEnemy)) CurrentEnemy->DispatcherStartTurn.Broadcast();
		}
	}
}

ALifeHuman* ABattleGameMode::AccessLifeByPlayerCode(FString PlayerCode)
{
	for (ALifeHuman* LifeHuman : PartyMembers)
	{
		if (LifeHuman->PlayerCode == PlayerCode) return LifeHuman;
	}
	return nullptr;
}

ALife* ABattleGameMode::AccessLifeByCode(FString Code)
{
	if (Code.StartsWith("Enemy"))
	{
		TArray<FString> Parts;
		Code.ParseIntoArray(Parts, TEXT("_"));
		if (Parts.Num()>1)
		{
			int32 EnemyIdx = FCString::Atoi(*Parts[1]);
			return Enemies[EnemyIdx];
		} else
		{
			return nullptr;
		}
	}
	return AccessLifeByPlayerCode(Code);
}

void ABattleGameMode::SetCurrentSkill(FRowSkill Skill)
{
	UE_LOG(LogTemp, Log, TEXT("GameMode Current Skill Changed -- %s"), *Skill.Name.ToString());
	this->CurrentSkill = Skill;
}

void ABattleGameMode::SetCurrentSkillRecord(FRowSkillRecord Skill)
{
	UE_LOG(LogTemp, Log, TEXT("GameMode Current Skill Record Changed -- %s"), *Skill.SkillId.ToString());
	this->CurrentSkillRecord = Skill;
}

/**
 * 스마트하게 공격할 대상을 찾는다.
 * 머리아프다..
 */
void ABattleGameMode::SelectWeakestPlayer()
{
	int32 LowestHp = 99999;
	ALifeHuman* LowestHpLife = nullptr;
	TArray<ALifeHuman*> PlayersHavingWeakPoint = {};
	this->CurrentSkillTargets = {}; //일단 비워놓고 시작

	// 만약 속성 공격이면..
	if (this->CurrentSkill.Elemental>0)
	{
		// 일단 약점이 잇는 캐릭터를 찾는다.
		for (ALifeHuman* LifeHuman : PartyMembers)
		{
			if (LifeHuman->LifeStatComponent->GetHp() <= 0) continue;
			if ((this->CurrentSkill.Elemental==1 && LifeHuman->LifeStatComponent->GetElementalFire()==-1)
				||(this->CurrentSkill.Elemental==2 && LifeHuman->LifeStatComponent->GetElementalIce()==-1)
				||(this->CurrentSkill.Elemental==3 && LifeHuman->LifeStatComponent->GetElementalThunder()==-1)
				||(this->CurrentSkill.Elemental==4 && LifeHuman->LifeStatComponent->GetElementalWind()==-1)
				||(this->CurrentSkill.Elemental==5 && LifeHuman->LifeStatComponent->GetElementalHoly()==-1)
				||(this->CurrentSkill.Elemental==6 && LifeHuman->LifeStatComponent->GetElementalDarkness()==-1)
			)
			{
				PlayersHavingWeakPoint.Add(LifeHuman);
			}
		}
	}
	
	//Hp가 젤 낮은 캐릭을 찾는다.
	for (ALifeHuman* LifeHuman : PartyMembers)
	{
		if (LifeHuman->LifeStatComponent->GetHp() <= 0) continue;
		if (LifeHuman->LifeStatComponent->GetHp() < LowestHp)
		{
			LowestHp = LifeHuman->LifeStatComponent->GetHp();
			LowestHpLife = LifeHuman;
		}
	}

	if (this->CurrentSkill.Elemental==0) // 물리 공격이면 Hp가 젤 낮은 캐릭을 스킬 타겟으로 설정
	{
		this->CurrentSkillTargets = {LowestHpLife};
	} else
	{
		// 마법공격이면 약점인 애들 중에 한명을 랜덤하게 선택한다.
		if (PlayersHavingWeakPoint.Num() > 0)
		{
			int32 RandomIdx = FMath::RandRange(0, PlayersHavingWeakPoint.Num()-1);
			this->CurrentSkillTargets = {PlayersHavingWeakPoint[RandomIdx]};
		} else
		{
			// 만약 약점가진애가 없으면.. 제일 피 낮은애를 공격한다.
			this->CurrentSkillTargets = {LowestHpLife};
		}
	}
}

void ABattleGameMode::SelectAllPlayer()
{
	TArray<ALifeHuman*> PlayersHavingWeakPoint = {};
	for (ALifeHuman* LifeHuman : PartyMembers)
	{
		if (LifeHuman->LifeStatComponent->GetHp() <= 0) continue;
		this->CurrentSkillTargets.Add(LifeHuman);
	}
}


void ABattleGameMode::ApplyDamage()
{
	ALife* CurrentTurnLife = AccessLifeByCode(this->CurrentTurnTarget);
	if (IsValid(CurrentTurnLife) && this->CurrentSkillTargets.Num() > 0)
	{
		for (ALife* Target : this->CurrentSkillTargets)
		{
			// 씨발같은 상수를 언젠가 리팩토링 해야할텐데.. 
			int32 MinDamage = (CurrentTurnLife->LifeStatComponent->GetStr() +  + this->CurrentSkill.BaseDamage/10.0f)*(2 + (CurrentTurnLife->LifeStatComponent->GetCon()/80));
			int32 MaxDamage = (CurrentTurnLife->LifeStatComponent->GetStr() + this->CurrentSkill.BaseDamage/7.0f)*3.5f;
			// 랜덤 데미지 계산
			float BaseDamage = static_cast<float>(FMath::RandRange(MinDamage, MaxDamage)) * (static_cast<float>(this->CurrentSkill.BaseDamage)/100.0f);
			
			// 방어력 적용
			int32 Defense = Target->LifeStatComponent->GetDef() * BattleConstants::DEFENSE_MULTIPLIER;
			int32 FinalDamage = FMath::Max(1, BaseDamage - Defense);

			if (Target->GetIsDefend())
			{
				FinalDamage = FMath::Max(1, FMath::Floor(FinalDamage / 2));
			}

			bool bIsWeak = false;
			bool bIsImmune = false;
			this->bIsCurrentTurnOneMore = false;

			if (
			this->CurrentSkill.Elemental == 1 && Target->LifeStatComponent->GetElementalFire() < 0 ||
			this->CurrentSkill.Elemental == 2 && Target->LifeStatComponent->GetElementalIce() < 0 ||
			this->CurrentSkill.Elemental == 3 && Target->LifeStatComponent->GetElementalThunder() < 0 ||
			this->CurrentSkill.Elemental == 4 && Target->LifeStatComponent->GetElementalWind() < 0 ||
			this->CurrentSkill.Elemental == 5 && Target->LifeStatComponent->GetElementalHoly() < 0 ||
			this->CurrentSkill.Elemental == 6 && Target->LifeStatComponent->GetElementalDarkness() < 0
			)
			{
				bIsWeak = true;
				if (Target->GetIsDown())
				{
					this->bIsCurrentTurnOneMore = false;
				} else
				{
					if (this->CurrentSkillTargets.Num() == 1) this->bIsCurrentTurnOneMore = true;
					Target->MakeDown();
				}
			}

			if (
			this->CurrentSkill.Elemental == 1 && Target->LifeStatComponent->GetElementalFire() == 1 ||
			this->CurrentSkill.Elemental == 2 && Target->LifeStatComponent->GetElementalIce() == 1 ||
			this->CurrentSkill.Elemental == 3 && Target->LifeStatComponent->GetElementalThunder() == 1 ||
			this->CurrentSkill.Elemental == 4 && Target->LifeStatComponent->GetElementalWind() == 1 ||
			this->CurrentSkill.Elemental == 5 && Target->LifeStatComponent->GetElementalHoly() == 1 ||
			this->CurrentSkill.Elemental == 6 && Target->LifeStatComponent->GetElementalDarkness() == 1
			)
			{
				bIsImmune = true;
			}

			if (bIsWeak)
			{
				FinalDamage *= BattleConstants::WEAK_DAMAGE_MULTIPLIER;
			} else if (bIsImmune)
			{
				FinalDamage *= BattleConstants::IMMUNE_DAMAGE_MULTIPLIER;
			}

			// Unreal Engine 기본 데미지 시스템 사용
			UGameplayStatics::ApplyDamage(
				Target,           // DamagedActor
				FinalDamage,                         // BaseDamage
				CurrentTurnLife->GetController(),    // EventInstigator
				CurrentTurnLife,                     // DamageCauser
				bIsWeak ? UDamageTypeWeak::StaticClass() : UDamageType::StaticClass()           // DamageType
			);

			this->DispatcherLifeHit.Broadcast();
			if (Target->LifeStatComponent->GetHp()<=0)
			{
				FTimerHandle Th;
				FVector SavedTargetPos = Target->GetActorLocation();
				GetWorldTimerManager().SetTimer(Th, [this,SavedTargetPos]()
				{
					if (this->bEndGame) return;
					ALife* CurrentTurnLife = AccessLifeByCode(this->CurrentTurnTarget);
					if (IsValid(CurrentTurnLife))
					{
						this->MainCam->SeePlayerBackToEnemy(CurrentTurnLife->GetActorLocation(), SavedTargetPos);
					}
				},3.0f,false);
				// handle death
				//UE_LOG(LogTemp,Log,TEXT("[BattleGameMode] : Handle %s death"),*Target->GetTmpCode());
				this->DeleteFromOrderedList(Target->GetTmpCode());
			}
		}
	}
}

// ==================================================
// Turn Related Things
// ==================================================

void ABattleGameMode::DeleteFromOrderedList(FString TmpCode)
{
	bool already = false;
	
	if (!this->DeadList.Contains(TmpCode))
	{
		//UE_LOG(LogTemp,Log,TEXT("[BattleGameMode] : %s (%s) DEAD. add to dead list."),*this->CurrentSkillTargets->GetName(), *TmpCode);
		this->DeadList.Add(TmpCode);
	}else
	{
		already = true;
	}
	if (already) return;

	// count leftover enemies

	if (TmpCode.StartsWith("Enemy_"))
	{
		NumDeadEnemies ++;
	}

	if (TmpCode.StartsWith("Enemy_"))
	{
		this->SelectedEnemy = GetFirstAliveEnemy();
		UE_LOG(LogTemp,Log,TEXT("[BattleGameMode] : Now Auto Selected Enemy is %d"),this->SelectedEnemy);
	}
	
}

/**
 * 게임이 끝났을 때 리워드를 준다.
 */
void ABattleGameMode::ApplyRewards()
{
	// 리워드 경험치 계산
	for (ALifeEnemy* Enemy : Enemies)
	{
		if (IsValid(Enemy)) this->Reward_Exp += Enemy->LifeStatComponent->GetHpMax() / 3;
	}
	// 리워드 경험치 어플라이
	for (ALifeHuman* Member : PartyMembers)
	{
		if (Member->LifeStatComponent->GetHp() > 0) // 살아있는 멤버만..
		{
			Member->LifeStatComponent->IncreaseExp(Reward_Exp);
		}
	}
	// 리워드 돈 계산
	for (ALifeEnemy* Enemy : Enemies)
	{
		float MinBtc = static_cast<float>(Enemy->LifeStatComponent->GetHpMax())/3000.0f;
		float MaxBtc = static_cast<float>(Enemy->LifeStatComponent->GetHpMax())/2000.0f;
		this->Reward_Btc += UKismetMathLibrary::RandomFloatInRange(MinBtc, MaxBtc);
	}
	// 리워드 돈 어플라이
	MyGameInstance->AddMoneyBtc(this->Reward_Btc);
	// 리워드 아이템 어플라이
}


void ABattleGameMode::EndGame()
{
	this->ApplyRewards();
	this->bEndGame = true;
	
	ALifeHuman* LastMan = AccessLifeByPlayerCode(this->CurrentTurnTarget);
	UE_LOG(LogTemp,Log,TEXT("[BattleGameMode] : End Game, %s last man"), *this->CurrentTurnTarget);

	// 모든 Ion 타이머를 안전하게 정지 및 제거
	for (FTimerHandle& TimerHandle : this->IonTimerHandles)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
	this->IonTimerHandles.Empty();

	
	FTimerHandle Th;
	GetWorldTimerManager().SetTimer(Th, [this, LastMan]()
	{
		this->MainCam->SeePlayerCenterToMargin(LastMan->GetActorLocation());
		if (IsValid(LastMan->GetMontageWin()))
		{
			LastMan->PlayAnimMontage(LastMan->GetMontageWin(), 1.0f);
		}
	}, 1.5f, false);

	
	
}


void ABattleGameMode::EndTurn()
{
	
	UE_LOG(LogTemp, Warning, TEXT("BattleGameMode :: End turn of %s"), *this->CurrentTurnTarget);
	this->TurnCount++;
	this->CurrentTurnTarget = PartyOrder[TurnCount % PartyOrder.Num()];

	UE_LOG(LogTemp, Warning, TEXT("BattleGameMode :: Try to start turn of %s"), *this->CurrentTurnTarget);
	if (DeadList.Contains(this->CurrentTurnTarget))
	{
		UE_LOG(LogTemp, Warning, TEXT("BattleGameMode :: turn of %s already dead. retry.."), *this->CurrentTurnTarget);
		this->EndTurn();
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("BattleGameMode :: Turn : %d, Current Party Numbers! %d, Dead List : %d"), this->TurnCount,PartyOrder.Num(), DeadList.Num());
		// 현재 턴 종료
		this->DispatcherGameModeTurnEnd.Broadcast();
		this->InitNewTurn();
	}
}

void ABattleGameMode::InitNewTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("BattleGameMode :: InitNewTurn of %s"), *CurrentTurnTarget);

	if (this->CurrentTurnTarget.IsEmpty()) return;

	// 다음 턴 시작

	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();
	
	if (this->CurrentTurnTarget.StartsWith("Enemy_"))
	{
		this->bIsPlayerSideTurn = false;
		ALifeEnemy* CurrentEnemy = Cast<ALifeEnemy>(AccessLifeByCode(CurrentTurnTarget));
		if (IsValid(CurrentEnemy))
		{
			CurrentEnemy->SetMyTurn(true);
			CurrentEnemy->DispatcherStartTurn.Broadcast();
		}

		// 만약 다운 상태였다면 회복
		if (CurrentEnemy->GetIsDown())
		{
			CurrentEnemy->RestoreDown();
		}
	} else
	{
		this->bIsPlayerSideTurn = true;
		ALife* CurrentPlayer = AccessLifeByCode(CurrentTurnTarget);

		// 만약 다운 상태라면 화복
		if (CurrentPlayer->GetIsDown()) CurrentPlayer->RestoreDown();
	}
	
	this->bIsCurrentTurnOneMore = false;
	
	this->AdjustCam();
	this->DispatcherGameModeTurnStart.Broadcast();
}

void ABattleGameMode::CurrentTurnDefend()
{
	ALife* CurrentTurnLife = AccessLifeByCode(CurrentTurnTarget);

	if (bIsPlayerSideTurn==false)
	{
		UDigitalBleedGameInstance* GI = Cast<UDigitalBleedGameInstance>(GetGameInstance());
		GI->ShowToast(FText::FromString("UI_ENEMY_COMMAND_DEFEND"));
	}
	
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBack(CurrentTurnLife->GetActorLocation());
		CurrentTurnLife->Defend();
	}
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		this->EndTurn();
	}, 1.0f, false);
}