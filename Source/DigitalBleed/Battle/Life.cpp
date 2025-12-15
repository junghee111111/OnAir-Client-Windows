// Fill out your copyright notice in the Description page of Project Settings.


#include "Life.h"

#include "LifeEquipComponent.h"
#include "LifeStatComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ALife::ALife()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	this->LifeEquipComponent = CreateDefaultSubobject<ULifeEquipComponent>("Life Equip Component");
	this->LifeStatComponent = CreateDefaultSubobject<ULifeStatComponent>("Life Stat Component");
}

// Called when the game starts or when spawned
void ALife::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

