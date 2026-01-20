// 2026.01.13 재작성 시작, Written By Junghee Wang

#include "BattleGameMode.h"

#include "BattleGamePlayerState.h"
#include "Battle/LifeHuman.h"
#include "Battle/Camera/BattleMainCam.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "System/DigitalBleedGameInstance.h"
#include "UI/Battle/WholeBattleUI.h"

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
		Enemies.Add(Enemy);
	}
	if (this->LifeEnemies.Num() > 1)
	{
		// Spawn in circle shape with same degrees in 360
		float AngleBetweenEnemies = PI*2 / this->LifeEnemies.Num();
		float distance = 200.0f;
		for (int i = 0; i < this->LifeEnemies.Num(); i++)
		{
			float Angle = AngleBetweenEnemies * i;
			FVector SpawnLocation = FVector(distance * FMath::Cos(Angle), distance * FMath::Sin(Angle), 0);

			FRotator LookRotation = (SpawnLocation-FVector(0,0,0)).Rotation();
			FRotator SpawnRotation = FRotator(0, LookRotation.Yaw, 0);
			ALifeEnemy* Enemy = GetWorld()->SpawnActor<ALifeEnemy>(this->LifeEnemies[i], SpawnLocation, SpawnRotation);
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
			AActor* SpawnedCharacter = GetWorld()->SpawnActor(DefaultPawnClass, &SpawnTransform, SpawnParams);
			
			ALifeHuman* LifeHuman = Cast<ALifeHuman>(SpawnedCharacter);
			if (IsValid(LifeHuman))
			{
				// PC->Possess(LifeHuman);
				LifeHuman->PlayerCode = GI->GetPartyMembers()[i];
				PartyMembers.Add(LifeHuman);
			}

			UE_LOG(
			LogTemp, Warning,
			TEXT("BattleGameMode :: SpawnPlayer at Pos : %d, Num : %d"),
			i, PlayerControllers.Num());
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
	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();

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
	
	// 첫 번째 플레이어(나) 추가
	if (IsValid(FirstPC))
	{
		FirstPC->SetInputMode(FInputModeUIOnly());
		FirstPC->bShowMouseCursor = true;
		ALifeHuman* MyLifeHuman = Cast<ALifeHuman>(FirstPC->GetPawn());
		if (IsValid(MyLifeHuman))
		{
			MyLifeHuman->PlayerCode = this->MyGameInstance->GetPartyMembers()[0]; // 첫 번째 PlayerCode 설정
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
	}, 0.5f, false);
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		this->InitializeUI();
		this->InitNewTurn();
	}, 3.0f, false);
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
			PartyOrder.Add(FString::Printf(TEXT("Enemy_%d"), MobIdx));
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
	UDigitalBleedGameInstance* GI = Cast<UDigitalBleedGameInstance>(GetGameInstance());
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnTarget);
	this->MainCam->SeePlayerBackToEnemy(CurrentTurnLife->GetActorLocation(), Enemies[0]->GetActorLocation());
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

void ABattleGameMode::CameraSee_SkillTarget_Angle1()
{
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBackToEnemy_Angle1(CurrentTurnLife->GetActorLocation(), this->CurrentSkillTarget->GetActorLocation());
	}
}

void ABattleGameMode::CameraSee_SkillTarget_Angle2()
{
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBackToEnemy_Angle2(CurrentTurnLife->GetActorLocation(), this->CurrentSkillTarget->GetActorLocation());
	}
}

void ABattleGameMode::CameraSee_SkillTarget_Angle3()
{
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnTarget);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBackToEnemy_Angle3(CurrentTurnLife->GetActorLocation(), this->CurrentSkillTarget->GetActorLocation());
	}
}

void ABattleGameMode::StartSelectEnemyMode()
{
	this->bEnemySelectMode = true;
	UDigitalBleedGameInstance* GI = Cast<UDigitalBleedGameInstance>(GetGameInstance());
	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();
	if (IsValid(GI))
	{
		GI->ShowToast(FText::FromString("BATTLE_SELECT_TARGET"));
		FirstPC->SetInputMode(FInputModeGameOnly());
		FirstPC->bShowMouseCursor = false;
		ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
		ALifeEnemy* SelectedEnemyLife = Enemies[SelectedEnemy];
		this->CurrentSkillTarget = SelectedEnemyLife;
		this->MainCam->SeePlayerBackToEnemy(CurrentLife->GetActorLocation(), SelectedEnemyLife->GetActorLocation());

		for (ALifeEnemy* Enemy : Enemies)
		{
			Enemy->ShowHpBar();
		}
		
	}
}

void ABattleGameMode::SelectNextEnemy()
{
	SelectedEnemy++;
	if (SelectedEnemy >= LifeEnemies.Num())
	{
		SelectedEnemy = 0;
	}
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	ALifeEnemy* SelectedEnemyLife = Enemies[SelectedEnemy];
	this->MainCam->SeePlayerBackToEnemy(CurrentLife->GetActorLocation(), SelectedEnemyLife->GetActorLocation());
	this->CurrentSkillTarget = SelectedEnemyLife;
}

void ABattleGameMode::SelectPrevEnemy()
{
	SelectedEnemy--;
	if (SelectedEnemy < 0)
	{
		SelectedEnemy = LifeEnemies.Num()-1;
	}
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	ALifeEnemy* SelectedEnemyLife = Enemies[SelectedEnemy];
	this->MainCam->SeePlayerBackToEnemy(CurrentLife->GetActorLocation(), SelectedEnemyLife->GetActorLocation());
	this->CurrentSkillTarget = SelectedEnemyLife;
}

void ABattleGameMode::EndSelectEnemyMode()
{
	this->bEnemySelectMode = false;
	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	if (IsValid(FirstPC))
	{
		FirstPC->SetInputMode(FInputModeUIOnly());
		FirstPC->bShowMouseCursor = true;
	}
	for (ALifeEnemy* Enemy : Enemies)
	{
		Enemy->HideHpBar();
	}
	this->MainCam->SeePlayerBack(CurrentLife->GetActorLocation());
}

void ABattleGameMode::ExecuteSkill()
{
	this->bIsSkillPlaying = true;
	this->EndSelectEnemyMode();

	if (!IsValid(this->CurrentSkillTarget))
	{
		this->EndTurn();
		return;
	}

	if (this->CurrentSkill.Name.IsEmpty())
	{
		this->EndTurn();
		return;
	}
	
	APlayerController* FirstPC = GetWorld()->GetFirstPlayerController();
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	
	if (IsValid(FirstPC))
	{
		
		FirstPC->SetInputMode(FInputModeUIOnly());
		FirstPC->bShowMouseCursor = true;
		
	}
	this->MainCam->GoTowardsTarget(CurrentLife->GetActorLocation(), this->CurrentSkillTarget->GetActorLocation());
	CurrentLife->ExecSkill(this->CurrentSkillTarget, this->CurrentSkill, this->CurrentSkillRecord);
	
	this->CurrentSkillTarget->ShowHpBar();
	CurrentLife->ShowHpBar();

	//스킬네임 토스트로 표출
	FString SkillNameKey = FString::Printf(TEXT("%s_TITLE"), *this->CurrentSkillRecord.SkillId.ToString());
	this->MyGameInstance->ShowToastSkillName(FText::FromString(SkillNameKey));
}

void ABattleGameMode::EndSkill()
{
	ALife* CurrentLife = this->AccessLifeByCode(this->CurrentTurnTarget);
	
	this->CurrentSkillTarget->HideHpBar();
	CurrentLife->HideHpBar();
	
	this->bIsSkillPlaying = false;
	this->EndTurn();
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
	this->CurrentSkill = Skill;
}

void ABattleGameMode::SetCurrentSkillRecord(FRowSkillRecord Skill)
{
	this->CurrentSkillRecord = Skill;
}

void ABattleGameMode::SelectPlayerLowestHp()
{
	int32 LowestHp = 99999;
	ALifeHuman* LowestHpLife = nullptr;
	for (ALifeHuman* LifeHuman : PartyMembers)
	{
		if (LifeHuman->LifeStatComponent->GetHp() < LowestHp)
		{
			LowestHp = LifeHuman->LifeStatComponent->GetHp();
			LowestHpLife = LifeHuman;
		}
	}

	if (IsValid(LowestHpLife))
	{
		this->CurrentSkillTarget = LowestHpLife;
	}
}

void ABattleGameMode::ApplyDamage()
{
	ALife* CurrentTurnLife = AccessLifeByCode(this->CurrentTurnTarget);
	if (IsValid(CurrentTurnLife) && IsValid(this->CurrentSkillTarget))
	{
		int32 MinDamage = CurrentTurnLife->LifeStatComponent->GetStr()*(2 + (CurrentTurnLife->LifeStatComponent->GetCon()/100));
		int32 MaxDamage = CurrentTurnLife->LifeStatComponent->GetStr()*3.5f;
		// 랜덤 데미지 계산
		int32 BaseDamage = FMath::RandRange(MinDamage, MaxDamage);
		
		// 방어력 적용
		int32 Defense = this->CurrentSkillTarget->LifeStatComponent->GetDef() * 1.5f;
		int32 FinalDamage = FMath::Max(1, BaseDamage - Defense);

		if (this->CurrentSkillTarget->GetIsDefend())
		{
			FinalDamage = FMath::Max(1, FMath::Floor(FinalDamage / 2));
		}

		// Unreal Engine 기본 데미지 시스템 사용
		UGameplayStatics::ApplyDamage(
			this->CurrentSkillTarget,           // DamagedActor
			FinalDamage,                         // BaseDamage
			CurrentTurnLife->GetController(),    // EventInstigator
			CurrentTurnLife,                     // DamageCauser
			UDamageType::StaticClass()          // DamageTypeClass
		);
		
		// 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamage: %s -> %s, Damage: %d (Base: %d, Def: %d)"),
			*CurrentTurnLife->GetName(),
			*this->CurrentSkillTarget->GetName(),
			FinalDamage,
			BaseDamage,
			Defense);
	}
}

// ==================================================
// Turn Related Things
// ==================================================

void ABattleGameMode::EndTurn()
{
	this->TurnCount++;
	this->CurrentTurnTarget = PartyOrder[TurnCount % PartyOrder.Num()];
	// 현재 턴 종료
	this->DispatcherGameModeTurnEnd.Broadcast();
	this->InitNewTurn();
}

void ABattleGameMode::InitNewTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("BattleGameMode :: InitNewTurn of %s"), *CurrentTurnTarget);

	if (this->CurrentTurnTarget.IsEmpty()) return;

	// 다음 턴 시작
	if (this->CurrentTurnTarget.StartsWith("Enemy_"))
	{
		this->bIsPlayerSideTurn = false;
		ALifeEnemy* CurrentEnemy = Cast<ALifeEnemy>(AccessLifeByCode(CurrentTurnTarget));
		if (IsValid(CurrentEnemy))
		{
			CurrentEnemy->SetMyTurn(true);
			CurrentEnemy->DispatcherStartTurn.Broadcast();
		}
	} else
	{
		this->bIsPlayerSideTurn = true;
		// ALife* CurrentPlayer = AccessLifeByCode(CurrentTurnTarget);
	}
	
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