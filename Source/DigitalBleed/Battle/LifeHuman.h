// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Life.h"
#include "LifeHuman.generated.h"

UCLASS()
class DIGITALBLEED_API ALifeHuman : public ALife
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALifeHuman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
