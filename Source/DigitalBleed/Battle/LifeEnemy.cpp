// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeEnemy.h"


// Sets default values
ALifeEnemy::ALifeEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALifeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALifeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALifeEnemy::SetMyTurn(bool New_bIsMyTurn)
{
	this->bIsMyTurn = New_bIsMyTurn;
}

