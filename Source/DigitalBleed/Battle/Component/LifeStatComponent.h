// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGITALBLEED_API ULifeStatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Str = 10; //공격
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Def = 10; //방어
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Int = 10; //명중
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Spd = 7; //속력
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Luk = 10; //크리
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Con = 10; //회피

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Hp = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 HpMax = 100;

	float Hemoglobin = 10.0f;
	float Sodium = 140.0f;
	float Potassium = 4.5f;

	// -1 : Weak, 0 : Normal, 1 : Immune, 2 : Nothing, 3 : Reflect, 4 : Absorb
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ElementalFire = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ElementalIce = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ElementalThunder = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ElementalWind = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ElementalHoly = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ElementalDarkness = 0;

public:
	// Sets default values for this component's properties
	ULifeStatComponent();

	UFUNCTION(BlueprintCallable)
	int32 GetSpd() const { return this->Spd;}

	UFUNCTION(BlueprintCallable)
	int32 GetHp() const { return this->Hp; }

	UFUNCTION(BlueprintCallable)
	int32 GetStr() const { return this->Str; }

	UFUNCTION(BlueprintCallable)
	int32 GetCon() const { return this->Con; }

	UFUNCTION(BlueprintCallable)
	int32 GetDef() const { return this->Def; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
};
