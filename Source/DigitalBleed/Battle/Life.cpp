// Fill out your copyright notice in the Description page of Project Settings.


#include "Life.h"

#include "./Component/LifeEquipComponent.h"
#include "./Component/LifeStatComponent.h"
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

void ALife::Defend()
{
	if (IsValid(this->MontageDefend))
	{
		USkeletalMeshComponent* MeshComp = GetMesh();
		if (IsValid(MeshComp) && MeshComp->GetAnimInstance())
		{
			MeshComp->GetAnimInstance()->Montage_Play(MontageDefend, 1.0f);
		}
	}
}

void ALife::ResetAnim()
{
	// montage stop all
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (IsValid(MeshComp) && MeshComp->GetAnimInstance())
	{
		MeshComp->GetAnimInstance()->Montage_Stop(0);
	}
}

