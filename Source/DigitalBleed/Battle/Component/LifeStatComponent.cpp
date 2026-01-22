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

void ULifeStatComponent::DecreaseIons()
{
	if (this->Hp <= 0) return;

	if (this->Potassium >= 0.01f) this->Potassium -= 0.01f;
	if (this->Sodium >= 0.2f) this->Sodium -= 0.2f;
	if (this->Hemoglobin >= 0.1f) this->Hemoglobin -= 0.1f;

	if (this->Potassium<3.5f || this->Potassium > 5.5f)
	{
		this->Hp -=5;
	}

	if (this->Sodium<135.0f || this->Sodium > 145.0f)
	{
		this->Hp --;
	}

	if (this->Sodium<125.0f || this->Sodium > 155.0f)
	{
		this->Hp --;
	}

	if (this->Hemoglobin<10.0f || this->Hemoglobin > 14.0f)
	{
		this->Hp -= 2;
	}
	
	
}

// Called when the game starts
void ULifeStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

