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
	int32 Str = 10; //공격
	int32 Def = 10; //방어
	int32 Int = 10; //명중
	int32 Spd = 10; //속력
	int32 Luk = 10; //크리
	int32 Con = 10; //회피

	int32 Hp = 100; 
	int32 HpMax = 100;

	float Hemoglobin = 10.0f;
	float Sodium = 140.0f;
	float Potasium = 4.5f;

public:
	// Sets default values for this component's properties
	ULifeStatComponent();

	UFUNCTION(BlueprintCallable)
	int32 GetSpd() const { return this->Spd;}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
