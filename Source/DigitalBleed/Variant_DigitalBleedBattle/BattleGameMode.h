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
	

protected:
	void AdjustCam();
	
	UPROPERTY()
	TArray<APlayerController*> PlayerControllers;

	void SpawnPlayers();

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	int32 CurrentPlayerIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	int32 MaxPlayersNum = 4;

	UPROPERTY(BlueprintReadOnly)
	bool bIsPlayerSideTurn = true;

	UPROPERTY(BlueprintReadOnly)
	bool bEnemySelectMode = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSkillPlaying = false;

	UPROPERTY(BlueprintReadOnly)
	FString CurrentTurnTarget = "";

	UPROPERTY(BlueprintReadOnly)
	ALife* CurrentSkillTarget = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 TurnCount = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> PartyOrder;

	UPROPERTY(BlueprintReadOnly)
	TArray<ALifeHuman*> PartyMembers;

	UPROPERTY(BlueprintReadOnly)
	TArray<ALifeEnemy*> Enemies;

	UPROPERTY(BlueprintReadOnly)
	ABattleMainCam* MainCam = nullptr;

	UPROPERTY()
	UDigitalBleedGameInstance* MyGameInstance = nullptr;
	

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
	void CameraSee_SkillTarget_Angle1();

	UFUNCTION(BlueprintCallable)
	void CameraSee_SkillTarget_Angle2();

	UFUNCTION(BlueprintCallable)
	void CameraSee_SkillTarget_Angle3();

	UFUNCTION(BlueprintCallable)
	void StartSelectEnemyMode();

	UFUNCTION(BlueprintCallable)
	void SelectNextEnemy();

	UFUNCTION(BlueprintCallable)
	void SelectPrevEnemy();

	UFUNCTION(BlueprintCallable)
	void EndSelectEnemyMode();

	UFUNCTION(BlueprintCallable)
	void ExecuteSkill();

	UFUNCTION(BlueprintCallable)
	void EndSkill();

	UPROPERTY(BlueprintReadWrite)
	FRowSkill CurrentSkill;

	UPROPERTY(BlueprintReadWrite)
	FRowSkillRecord CurrentSkillRecord;

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

	UFUNCTION(BlueprintCallable)
	void SetCurrentSkill(FRowSkill Skill);

	UFUNCTION(BlueprintCallable)
	void SetCurrentSkillRecord(FRowSkillRecord Skill);

	UFUNCTION(BlueprintCallable)
	void SelectPlayerLowestHp();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage();
	
};
