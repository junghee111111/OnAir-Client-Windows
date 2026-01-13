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
		GetWorld()->SpawnActor<ALifeEnemy>(this->LifeEnemies[0], SpawnLocation, SpawnRotation);
	}
	if (this->LifeEnemies.Num() > 1)
	{
		// Spawn in circle shape with same degrees in 360
		float AngleBetweenEnemies = PI*2 / this->LifeEnemies.Num();
		float distance = 300.0f;
		for (int i = 0; i < this->LifeEnemies.Num(); i++)
		{
			float Angle = AngleBetweenEnemies * i;
			FVector SpawnLocation = FVector(distance * FMath::Cos(Angle), distance * FMath::Sin(Angle), 0);
			FRotator SpawnRotation = FRotator(0, FMath::RadiansToDegrees(Angle)-90, 0);
			GetWorld()->SpawnActor<ALifeEnemy>(this->LifeEnemies[0], SpawnLocation, SpawnRotation);
		}
	}
}


void ABattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	RegisterPlayerController(NewPlayer);
	if (PlayerControllers.Num() == 1)
	{
		NewPlayer->SetInputMode(FInputModeUIOnly());
		NewPlayer->bShowMouseCursor = true;
		SpawnPlayers();
	}
	CalculatePartyOrder();

	CurrentTurnPlayer = PartyOrder[0];
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		this->InitNewTurn();
	}, 2.0f, false);
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
		FRotator SpawnRotation = StartSpot->GetActorRotation();
		PC->GetPawn()->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
	}
}

void ABattleGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	PlayerControllers.Empty();
	CurrentPlayerIndex = 0;
	this->SpawnEnemies();
}

void ABattleGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
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

		if (PC)
		{
			AActor* StartSpot = this->GetPlayerStartByIndex(i);

			if (StartSpot == nullptr) continue;
			
			this->RegisterPlayerController(PC);
			FTransform SpawnTransform = StartSpot->GetActorTransform();
			AActor* SpawnedCharacter = GetWorld()->SpawnActor(DefaultPawnClass, &SpawnTransform, SpawnParams);
			ALifeHuman* LifeHuman = Cast<ALifeHuman>(SpawnedCharacter);
			if (IsValid(LifeHuman))
			{
				LifeHuman->PlayerCode = GI->GetPartyMembers()[i];
				PartyMembers.Add(LifeHuman);
			}

			UE_LOG(
			LogTemp, Warning,
			TEXT("BattleGameMode :: SpawnPlayer at Pos : %d, Num : %d"),
			i, PlayerControllers.Num());
			
			if (IsValid(SpawnedCharacter)) PC->Possess(Cast<APawn>(SpawnedCharacter));
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
		this->WholeBattleUI_Instance = CreateWidget<UWholeBattleUI>(PlayerControllers[0], this->WholeBattleUIClass);
		
		if (IsValid(this->WholeBattleUI_Instance))
		{
			this->WholeBattleUI_Instance->AddToViewport();
		}
	}
}

void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerControllers.Num() > 0)
	{
		this->InitializeMainCamera();
		this->InitializeUI();
	}
	
}

void ABattleGameMode::CalculatePartyOrder()
{
	// order party member as Spd desc order
	PartyOrder.Empty();
	TArray<ALifeHuman*> SortedMembers = PartyMembers;

	SortedMembers.Sort([](const ALifeHuman& A, const ALifeHuman& B)
	{
		return A.LifeStatComponent->GetSpd() > B.LifeStatComponent->GetSpd();
	});
	
	for (int i = 0; i < SortedMembers.Num(); i++)
	{
		PartyOrder.Add(SortedMembers[i]->PlayerCode);
	}
}

void ABattleGameMode::EndTurn()
{
	this->TurnCount++;
	this->CurrentTurnPlayer = PartyOrder[TurnCount % PartyOrder.Num()];
	UDigitalBleedGameInstance* GI = Cast<UDigitalBleedGameInstance>(GetGameInstance());
	GI->ShowToast(FText::FromString("DEBUG_ENDTURN"));
	this->InitNewTurn();
}

void ABattleGameMode::InitNewTurn()
{
	ALifeHuman* CurrentTurnLife = AccessLifeByPlayerCode(CurrentTurnPlayer);
	if (IsValid(CurrentTurnLife))
	{
		this->MainCam->SeePlayerBack(CurrentTurnLife->GetActorLocation());
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