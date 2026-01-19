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
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (IsValid(MeshComp) && MeshComp->GetAnimInstance())
	{
		this->AnimInstance = MeshComp->GetAnimInstance();
	}

	// See FVector(0,0,0)
	FRotator LookAtRotation = (GetActorLocation()-FVector(0,0,0)).Rotation();
	SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
}

void ALife::OnSkillMontageEnded(UAnimMontage* AnimMontage, bool bArg)
{
	this->AnimInstance->StopAllMontages(0.25f);
	
	this->TargetLocation = this->OriginalLocation;
	this->bIsMovingToTarget = true;
	this->bIsGoingOriginalPos = true;

	FRotator LookAtRotation = (this->OriginalLocation - GetActorLocation()).Rotation();
	SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
	
	this->AnimInstance->Montage_Play(this->MontageRun,1.0f);
	UE_LOG(LogTemp, Warning, TEXT("Skill Montage Ended"));
}

void ALife::ExecSkillMontages()
{
	

	int32 HitCount = FMath::RandRange(this->CurrentSkill.MinHitCount, this->CurrentSkill.MaxHitCount);

	switch (this->CurrentSkill.Elemental)
	{
	case 0:
		if (HitCount == 1)
		{
			this->AnimInstance->Montage_Play(this->MontagePhysAttack1,1.0f);
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this,&ALife::OnSkillMontageEnded);
			this->AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, this->MontagePhysAttack1);
		} else if (HitCount == 2)
		{
			this->AnimInstance->Montage_Play(this->MontagePhysAttack2,1.0f);
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this,&ALife::OnSkillMontageEnded);
			this->AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, this->MontagePhysAttack2);
		} else if (HitCount == 3)
		{
			this->AnimInstance->Montage_Play(this->MontagePhysAttack3,1.0f);
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this,&ALife::OnSkillMontageEnded);
			this->AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, this->MontagePhysAttack3);
		}
		break;
		
	}
	
}

// Called every frame
void ALife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타겟 위치로 이동 중
	if (bIsMovingToTarget)
	{
		FVector CurrentLocation = GetActorLocation();
		float Distance = FVector::Dist(CurrentLocation, TargetLocation);
		

		if (Distance <= 10.0f) // 목표 지점에 도달
		{
			this->AnimInstance->StopAllMontages(0.25f);
			SetActorLocation(TargetLocation);
			bIsMovingToTarget = false;
			
			this->bReadyForExecuteSkill = true;
			
			if (this->bIsGoingOriginalPos == true)
			{
				this->AnimInstance->StopAllMontages(0.1f);
				FRotator LookAtRotation = (FVector(0,0,0) - GetActorLocation()).Rotation();
				SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
				this->bIsGoingOriginalPos = false;
			} else if (this->bReadyForExecuteSkill)
			{
				this->ExecSkillMontages();
			}
		}
		else
		{
			// 타겟 위치로 이동 (보간)
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, 1000.0f);
			SetActorLocation(NewLocation);
		}
	}
}

void ALife::ExecSkill(ALife* TargetLife, FRowSkill Skill, FRowSkillRecord SkillRecord)
{
	if (!IsValid(TargetLife)) return;

	this->bReadyForExecuteSkill = false;
	// 원래 위치 저장
	this->OriginalLocation = GetActorLocation();
	
	// 타겟 위치 설정 (타겟 앞쪽으로 약간 떨어진 위치)
	FVector Direction = (TargetLife->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	this->TargetLocation = TargetLife->GetActorLocation() - Direction * 100.0f;
	
	this->bIsMovingToTarget = true;
	this->CurrentSkill = Skill;
	this->CurrentSkillRecord = SkillRecord;

	// 타겟을 바라보도록 회전
	FRotator LookAtRotation = (TargetLife->GetActorLocation() - GetActorLocation()).Rotation();
	SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));

	this->AnimInstance->Montage_Play(this->MontageRun,1.0f);
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

