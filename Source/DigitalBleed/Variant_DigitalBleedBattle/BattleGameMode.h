// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Battle/LifeEnemy.h"
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

	UFUNCTION()
	void RegisterPlayerController(APlayerController* PC);

	void InitializeMainCamera() const;
	void InitializeUI();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle|Enemy")
	TArray<TSubclassOf<class ALifeEnemy>> LifeEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle|UI")
	TSubclassOf<class UWholeBattleUI> WholeBattleUIClass;

	UPROPERTY(BlueprintReadOnly, Category = "Battle|UI")
	UWholeBattleUI* WholeBattleUI_Instance;
};
