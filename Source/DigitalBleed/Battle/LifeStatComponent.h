// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIGITALBLEED_API ULifeStatComponent : public UActorComponent
{
	GENERATED_BODY()

	int32 Att; //공격
	int32 Def; //방어
	int32 Con; //명중
	int32 Spd; //속력
	int32 Luk; //크리
	int32 Dod; //회피

public:
	// Sets default values for this component's properties
	ULifeStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
