// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeStatComponent.h"

#include "System/DigitalBleedGameInstance.h"


// Sets default values for this component's properties
ULifeStatComponent::ULifeStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void ULifeStatComponent::ManipulateHp(int32 Delta)
{
	if (this->Hp <= 0) return;
	if (this->Hp + Delta > this->HpMax) this->Hp = this->HpMax;
	else this->Hp += Delta;
	if (this->Hp + Delta < 0) this->Hp = 0;
}

void ULifeStatComponent::ManipulateHb(float Delta)
{
	if (this->Hp <= 0) return;
	if (this->Hemoglobin + Delta > 20.0f) this->Hemoglobin = 20.0f;
	else this->Hemoglobin += Delta;

	if (this->Hemoglobin + Delta < 0.0f) this->Hemoglobin = 0.0f;
}

void ULifeStatComponent::ManipulateSodium(float Delta)
{
	if (this->Hp <= 0) return;
	if (this->Sodium + Delta > 280.0f) this->Sodium = 280.0f;
	else this->Sodium += Delta;

	if (this->Sodium + Delta < 0.0f) this->Sodium = 0.0f;
}

void ULifeStatComponent::ManipulatePotassium(float Delta)
{
	if (this->Hp <= 0) return;
	if (this->Potassium + Delta > 9.0f) this->Potassium = 9.0f;
	else this->Potassium += Delta;

	if (this->Potassium < 0.0f) this->Potassium = 0.0f;
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

	// if (this->Potassium >= 0.01f) this->Potassium -= 0.01f;
	// if (this->Sodium >= 0.2f) this->Sodium -= 0.2f;
	if (this->Hemoglobin >= 0.1f) this->Hemoglobin -= 0.1f;

	if (this->Potassium<3.5f || this->Potassium > 5.5f)
	{
		this->Hp --;
	}

	if (this->Sodium<135.0f || this->Sodium > 145.0f)
	{
		this->Hp --;
	}
	
	if (this->Hemoglobin<10.0f || this->Hemoglobin > 14.0f)
	{
		this->Hp --;
	}
	
	
}

void ULifeStatComponent::IncreaseExp(int32 delta)
{
	UDigitalBleedGameInstance* GI = Cast<UDigitalBleedGameInstance>(GetWorld()->GetGameInstance());
	this->Exp += delta;

	if (IsValid(GI))
	{
		int32 MaxExp = GI->GetMaxExpForLevel(this->Level);
		if (this->Exp>=MaxExp)
		{
			this->Level++;
			this->Exp = this->Exp - MaxExp;
		}

		int32 MaxExpNextLevel = GI->GetMaxExpForLevel(this->Level);
		if (this->Exp >= MaxExp)
		{
			this->Exp = MaxExpNextLevel - 1;
		}
	}
}

// Called when the game starts
void ULifeStatComponent::BeginPlay()
{
	Super::BeginPlay();

	this->Hp = this->HpMax;
	// ...
	
}

