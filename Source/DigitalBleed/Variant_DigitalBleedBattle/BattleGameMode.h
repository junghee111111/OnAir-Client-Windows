// 2026.01.13 재작성 시작, Written By Junghee Wang

#pragma once

#include "CoreMinimal.h"
#include "Battle/LifeEnemy.h"
#include "Battle/LifeHuman.h"
#include "Battle/Camera/BattleMainCam.h"
#include "GameFramework/GameMode.h"
#include "BattleGameMode.generated.h"

/**
 * 
 */
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
	FString CurrentTurnTarget = "";

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
	

	UFUNCTION()
	void RegisterPlayerController(APlayerController* PC);

	void InitializeMainCamera();
	void InitializeUI();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle|Enemy")
	TArray<TSubclassOf<class ALifeEnemy>> LifeEnemies;

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

public:
	UFUNCTION(BlueprintCallable)
	void EndTurn();
	
	void InitNewTurn();

	UFUNCTION(BlueprintCallable)
	ALifeHuman* AccessLifeByPlayerCode(FString PlayerCode);
	ALife* AccessLifeByCode(FString Code);
};
