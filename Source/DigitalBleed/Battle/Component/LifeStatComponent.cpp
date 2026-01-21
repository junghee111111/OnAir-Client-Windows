// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeStatComponent.h"


// Sets default values for this component's properties
ULifeStatComponent::ULifeStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void ULifeStatComponent::GiveDamage(int32 Damage)
{
	if (this->Hp <= 0) return;

	if (this->Hp - Damage < 0)
	{
		
		this->Hp = 0;
	} else
	{
		this->Hp -= Damage;
	}
}

// Called when the game starts
void ULifeStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

