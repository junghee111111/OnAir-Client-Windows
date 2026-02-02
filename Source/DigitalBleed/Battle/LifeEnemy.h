// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Life.h"
#include "Data/StructItem.h"
#include "LifeEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartTurn);

UCLASS()
class DIGITALBLEED_API ALifeEnemy : public ALife
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALifeEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category="DGHS|Battle")
	bool bIsMyTurn;

	UPROPERTY(BlueprintReadWrite, Category="DGHS|Battle")
	FName EnemyId;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetMyTurn(bool New_bIsMyTurn);

	UFUNCTION(BlueprintCallable)
	bool IsMyTurn() const { return bIsMyTurn; }

	UPROPERTY(BlueprintAssignable, Category = "Turn")
	FOnStartTurn DispatcherStartTurn;
};
