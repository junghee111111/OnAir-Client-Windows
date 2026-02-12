// 2026.01.13 재작성 시작, Written By Junghee Wang

#pragma once

#include "CoreMinimal.h"
#include "Battle/LifeEnemy.h"
#include "Battle/LifeHuman.h"
#include "Battle/Camera/BattleMainCam.h"
#include "GameFramework/GameMode.h"
#include "System/DigitalBleedGameInstance.h"
#include "BattleGameMode.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLifeHit);

UCLASS()
class DIGITALBLEED_API ABattleGameMode : public AGameMode
{
	GENERATED_BODY()
	virtual void PostLogin(APlayerController* NewPlayer) override;
	AActor* GetPlayerStartByIndex(int32 PlayerIndex) const;
	void RestartPlayer(AController* NewPlayer) override;
	
public:
	ABattleGameMode();
	void SpawnEnemies();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void Tick(float DeltaSeconds) override;
	void StartIonTimer();


protected:
	void AdjustCam();
	
	UPROPERTY()
	TArray<APlayerController*> PlayerControllers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Sound")
	USoundBase* SFX_ItemUse = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Classes")
	TSubclassOf<ALifeHuman> Player_HYJClass = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Classes")
	TSubclassOf<ALifeHuman> Player_CJYClass = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Classes")
	TSubclassOf<ALifeHuman> Player_JARClass = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Classes")
	TSubclassOf<ALifeHuman> Player_PMSClass = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player Classes")
	TSubclassOf<ALifeHuman> Player_YJSClass = nullptr;

	void SpawnPlayers();
	
	// ========== Battle Rewards ==========
	UPROPERTY(BlueprintReadOnly)
	float Reward_Btc = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	int32 Reward_Exp = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<FRowItem> Reward_Items = {};
	// ========== Battle Rewards ==========

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	int32 CurrentPlayerIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	int32 MaxPlayersNum = 4;

	UPROPERTY(BlueprintReadOnly)
	bool bIsPlayerSideTurn = true;

	UPROPERTY(BlueprintReadOnly)
	bool bEnemySelectMode = false;

	UPROPERTY(BlueprintReadOnly)
	bool bPlayerSelectMode = false;

	UPROPERTY(BlueprintReadOnly)
	bool bCTScanMode = false;

	UPROPERTY(BlueprintReadWrite)
	bool bItemSelectedMode = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSkillPlaying = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsItemPlaying = false;

	UPROPERTY(BlueprintReadOnly)
	bool bEndGame = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCurrentTurnOneMore = false;

	UPROPERTY(BlueprintReadOnly)
	FString CurrentTurnTarget = "";

	UPROPERTY(BlueprintReadOnly)
	ALife* CurrentSkillTarget = nullptr;

	UPROPERTY(BlueprintReadOnly)
	ALife* CurrentItemTarget = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 TurnCount = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> PartyOrder;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> DeadList;

	UPROPERTY()
	int32 NumDeadEnemies = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<ALifeHuman*> PartyMembers;

	UPROPERTY(BlueprintReadOnly)
	TArray<ALifeEnemy*> Enemies;

	UPROPERTY(BlueprintReadOnly)
	ABattleMainCam* MainCam = nullptr;

	UPROPERTY()
	UDigitalBleedGameInstance* MyGameInstance = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AActor> LockOnIndicatorClass = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AActor* LockOnIndicator = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<FTimerHandle> IonTimerHandles = {};

	UFUNCTION()
	void RegisterPlayerController(APlayerController* PC);

	void InitializeMainCamera();
	void InitializeUI();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle|Enemy")
	TArray<TSubclassOf<ALifeEnemy>> LifeEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle|Enemy")
	int32 SelectedEnemy = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle|UI")
	TSubclassOf<class UWholeBattleUI> WholeBattleUIClass;

	UPROPERTY(BlueprintReadOnly, Category = "Battle|UI")
	UWholeBattleUI* WholeBattleUI_Instance;

	UFUNCTION()
	void CalculatePartyOrder();

	UFUNCTION(BlueprintCallable)
	void CTScan();

	UFUNCTION(BlueprintCallable)
	void CameraSeeEnemyOnly(int32 EnemyIdx);

	UFUNCTION(BlueprintCallable)
	void CameraSeeTurnOwner();

	UFUNCTION(BlueprintCallable)
	void StartSelectEnemyMode();

	UFUNCTION(BlueprintCallable)
	void SelectNextEnemy();

	UFUNCTION(BlueprintCallable)
	void SelectPrevEnemy();

	UFUNCTION(BlueprintCallable)
	void EndSelectEnemyMode();

	UFUNCTION(BlueprintCallable)
	void StartSelectPlayerMode();

	UFUNCTION(BlueprintCallable)
	void SelectNextPlayer();

	UFUNCTION(BlueprintCallable)
	void SelectPrevPlayer();

	UFUNCTION(BlueprintCallable)
	void EndSelectPlayerMode();
	
	int32 GetFirstAliveEnemy();
	int32 GetLastAliveEnemy();
	int32 GetEnemyIdxFromEnemyCode(FString EnemyCode);

	void ApplyItemEffect();
	UFUNCTION(BlueprintCallable)
	void ExecuteItem();

	UFUNCTION(BlueprintCallable)
	void ExecuteSkill();

	UPROPERTY(BlueprintReadWrite)
	FRowSkill CurrentSkill;

	UPROPERTY(BlueprintReadWrite)
	FRowSkillRecord CurrentSkillRecord;

	UPROPERTY(BlueprintReadWrite)
	FRowItem CurrentItem;

	UPROPERTY(BlueprintReadWrite)
	FRowItemRecord CurrentItemRecord;

	void DeleteFromOrderedList(FString TmpCode);
	
	void ApplyRewards();
	
	void EndGame();

public:
	UFUNCTION(BlueprintCallable)
	void EndTurn();
	
	void InitNewTurn();

	UFUNCTION(BlueprintCallable)
	void CurrentTurnDefend();

	UFUNCTION(BlueprintCallable)
	ALifeHuman* AccessLifeByPlayerCode(FString PlayerCode);
	ALife* AccessLifeByCode(FString Code);

	UPROPERTY(BlueprintAssignable, Category = "Turn")
	FOnTurnEnd DispatcherGameModeTurnEnd;

	UPROPERTY(BlueprintAssignable, Category = "Turn")
	FOnTurnStart DispatcherGameModeTurnStart;

	UPROPERTY(BlueprintAssignable, Category = "Turn")
	FOnLifeHit DispatcherLifeHit;

	UFUNCTION(BlueprintCallable)
	void SetCurrentSkill(FRowSkill Skill);

	UFUNCTION(BlueprintCallable)
	void SetCurrentSkillRecord(FRowSkillRecord Skill);

	UFUNCTION(BlueprintCallable)
	void SelectPlayerLowestHp();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage();

	UFUNCTION(BlueprintCallable)
	void CameraSee_SkillTarget_Angle1();

	UFUNCTION(BlueprintCallable)
	void CameraSee_SkillTarget_Angle2();

	UFUNCTION(BlueprintCallable)
	void CameraSee_SkillTarget_Angle3();

	UFUNCTION(BlueprintCallable)
	void CameraSee_SkillTarget_Angle4();

	UFUNCTION()
	ALife* GetSkillTarget() const { return this->CurrentSkillTarget; }

	UFUNCTION(BlueprintCallable)
	void EndSkill();
	
	UFUNCTION(BlueprintCallable)
	bool GetEnemySelectMode() const { return bEnemySelectMode; };

	UFUNCTION(BlueprintCallable)
	UWholeBattleUI* GetWholeBattleUI() const { return WholeBattleUI_Instance; }
	
};
