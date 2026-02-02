// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeStatComponent.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent))
class DIGITALBLEED_API ULifeStatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Level = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Exp = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Str = 10; //공격
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Def = 10; //방어
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Int = 10; //명중
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Spd = 10; //속력
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Luk = 10; //크리
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Con = 10; //회피

	UPROPERTY(BlueprintReadWrite)
	int32 Hp = 300;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 HpMax = 300;

	UPROPERTY(BlueprintReadWrite)
	float Hemoglobin = 12.0f;
	UPROPERTY(BlueprintReadWrite)
	float Sodium = 140.0f;
	UPROPERTY(BlueprintReadWrite)
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
	int32 GetHpMax() const { return this->HpMax; }

	UFUNCTION(BlueprintCallable)
	int32 GetStr() const { return this->Str; }

	UFUNCTION(BlueprintCallable)
	int32 GetCon() const { return this->Con; }

	UFUNCTION(BlueprintCallable)
	int32 GetDef() const { return this->Def; }

	UFUNCTION(BlueprintCallable)
	int32 GetLevel() const { return this->Level; }

	int32 GetElementalFire() const { return this->ElementalFire; }

	int32 GetElementalIce() const { return this->ElementalIce; }

	int32 GetElementalThunder() const { return this->ElementalThunder; }

	int32 GetElementalWind() const { return this->ElementalWind; }

	int32 GetElementalHoly() const { return this->ElementalHoly; }

	int32 GetElementalDarkness() const { return this->ElementalDarkness; }

	UFUNCTION(BlueprintCallable)
	void ManipulateHp(int32 Delta);

	UFUNCTION(BlueprintCallable)
	void ManipulateHb(float Delta);

	UFUNCTION(BlueprintCallable)
	void ManipulateSodium(float Delta);
	
	UFUNCTION(BlueprintCallable)
	void ManipulatePotassium(float Delta);
	
	UFUNCTION(BlueprintCallable)
	void GiveDamage(int32 Damage);
	
	void DecreaseIons();

	UFUNCTION(BlueprintCallable)
	void IncreaseExp(int32 delta);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
