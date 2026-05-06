// Fill out your copyright notice in the Description page of Project Settings.


#include "Life.h"

#include "LifeHuman.h"
#include "NiagaraFunctionLibrary.h"
#include "./Component/LifeEquipComponent.h"
#include "./Component/LifeStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Variant_DigitalBleedBattle/BattleGameMode.h"

// Sets default values
ALife::ALife()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	this->LifeEquipComponent = CreateDefaultSubobject<ULifeEquipComponent>("Life Equip Component");
	this->LifeStatComponent = CreateDefaultSubobject<ULifeStatComponent>("Life Stat Component");
	// HP Bar Widget Component 생성
	HpBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBarWidget"));
	HpBarWidgetComponent->SetupAttachment(RootComponent);
	
	// Widget Component 설정
	HpBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // Screen space로 설정 (항상 카메라를 향함)
	HpBarWidgetComponent->SetDrawSize(FVector2D(200.0f, 50.0f)); // 위젯 크기
	HpBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f)); // 캐릭터 머리 위
	HpBarWidgetComponent->SetVisibility(true);
}

// Called when the game starts or when spawned
void ALife::BeginPlay()
{
	Super::BeginPlay();
	this->MyGameMode = Cast<ABattleGameMode>(GetWorld()->GetAuthGameMode());
	
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (IsValid(MeshComp) && MeshComp->GetAnimInstance())
	{
		this->AnimInstance = MeshComp->GetAnimInstance();
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("MeshComponent or AnimInstance is not valid!"));
	}
	
	ALifeHuman* CanIHuman = Cast<ALifeHuman>(this);
	if (CanIHuman != nullptr)
	{
		// See FVector(0,0,0)
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]()
		{
			this->bIsMovingToTarget = true;
			this->TargetLocation = FVector(0,0,0);
			this->AnimInstance->Montage_Play(this->MontageRun,1.0f);
		},3.5f, false);
		
		FTimerHandle TimerHandle2;
		GetWorldTimerManager().SetTimer(TimerHandle2, [this]()
		{
			this->bIsMovingToTarget = false;
			this->AnimInstance->StopAllMontages(0.1f);
			FRotator LookAtRotation = (FVector(0,0,0)-GetActorLocation()).Rotation();
			SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
			//HpBarWidgetComponent->SetWidgetClass(HpBarWidgetClass);
		},4.0f, false);
	}
}

void ALife::LaunchProjectileToTarget(ALife* Target)
{
	if (!IsValid(Target) || !IsValid(this->CurrentSkill.ProjectileEffect))
	{
		return;
	}

	UNiagaraComponent* Projectile = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		this->CurrentSkill.ProjectileEffect,
		this->GetActorLocation(),
		this->GetActorRotation(),
		FVector(1.0f),
		false
	);

	if (!Projectile)
	{
		return;
	}

	const FVector StartLocation = this->GetActorLocation();
	const FVector EndLocation = Target->GetActorLocation();
	constexpr float Duration = 0.5f;

	TSharedPtr<FTimerHandle> TimerHandle = MakeShared<FTimerHandle>();
	TSharedPtr<float> ElapsedTime = MakeShared<float>(0.0f);

	GetWorldTimerManager().SetTimer(
		*TimerHandle,
		[this, Projectile, StartLocation, EndLocation, Duration, ElapsedTime, TimerHandle]() mutable
		{
			if (!UpdateProjectileMovement(Projectile, StartLocation, EndLocation, Duration, ElapsedTime))
			{
				CleanupProjectile(Projectile, TimerHandle);
			}
		},
		GetWorld()->GetDeltaSeconds(),
		true
	);
}

bool ALife::UpdateProjectileMovement(
	UNiagaraComponent* Projectile,
	const FVector& StartLocation,
	const FVector& EndLocation,
	float Duration,
	TSharedPtr<float> ElapsedTime)
{
	*ElapsedTime += GetWorld()->GetDeltaSeconds();
	const float Alpha = FMath::Clamp(*ElapsedTime / Duration, 0.0f, 1.0f);

	if (!Projectile->IsValidLowLevel() || !Projectile->IsRegistered())
	{
		return false;
	}

	const FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
	Projectile->SetWorldLocation(NewLocation);

	const FRotator NewRotation = (EndLocation - NewLocation).Rotation();
	Projectile->SetWorldRotation(NewRotation);

	return Alpha < 1.0f;
}

void ALife::CleanupProjectile(
	UNiagaraComponent* Projectile,
	TSharedPtr<FTimerHandle> TimerHandle)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile Timer Ended"));
	
	GetWorldTimerManager().ClearTimer(*TimerHandle);
	
	if (Projectile && Projectile->IsValidLowLevel())
	{
		Projectile->DestroyComponent();
	}
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
	UE_LOG(LogTemp, Warning, TEXT("ExecSkillMontages"));
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
	default: // 마법 공격!
		if (this->CurrentSkill.bIsAllAttack == true)
		{
			this->MyGameMode->CameraSee_SkillTargets_All();
		} else
		{
			this->MyGameMode->CameraSee_SkillTarget_Angle4();
		}
		
		
		if (IsValid(this->CurrentSkill.ProjectileEffect))
		{
			for (ALife* Target : this->MyGameMode->GetSkillTarget())
			{
				if (!IsValid(Target)) continue;

				LaunchProjectileToTarget(Target);
			}
		}

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (IsValid(this->CurrentSkill.HitEffect))
			{
				for (ALife* Target : this->MyGameMode->GetSkillTarget())
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						GetWorld(),
						this->CurrentSkill.HitEffect,
						Target->GetActorLocation(),
						this->GetActorRotation(),
						FVector(0.5f),
						true
					);
				}
			}
			this->MyGameMode->ApplyDamage();
		},0.5f, false);

		FTimerHandle TimerHandle2;
		GetWorldTimerManager().SetTimer(TimerHandle2, [this]()
		{
			this->MyGameMode->EndSkill();
		},2.5f, false);
		
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

			if (!this->CurrentSkill.Name.IsEmpty())
			{
				this->bReadyForExecuteSkill = true;
			}
			
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
			FRotator LookAtRotation = (TargetLocation - CurrentLocation).Rotation();
			SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
			// 타겟 위치로 이동 (보간)
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, 600.0f);
			SetActorLocation(NewLocation);
		}
	}
}

void ALife::ExecSkill(TArray<ALife*> Targets, FRowSkill Skill, FRowSkillRecord SkillRecord)
{
	UE_LOG(LogTemp, Log, TEXT("Life - %s : ExecSkill -- skill name : %s" ), *this->GetName(), *Skill.Name.ToString())
	if (Targets.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("  ==> Canceled!! No targets.." ))
		return;
	}
	
	this->CurrentSkill = Skill;
	this->CurrentSkillRecord = SkillRecord;

	

	// 단일 대상 공격이면, 타겟을 바라보도록 회전
	if (Skill.bIsAllAttack == false)
	{
		FRotator LookAtRotation = (Targets[0]->GetActorLocation() - GetActorLocation()).Rotation();
		SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
	}

	this->LifeStatComponent->ManipulateHp(Skill.CostHP*-1);
	// this->LifeStatComponent->ManipulateHb(Skill.CostHb*-1);
	this->LifeStatComponent->ManipulateSodium(Skill.CostNa*-1);
	this->LifeStatComponent->ManipulatePotassium(Skill.CostK*-1);
	
	if (Skill.Elemental == 0) // 물리 스킬
	{
		this->bReadyForExecuteSkill = false;
		this->bIsMovingToTarget = true;
		// 원래 위치 저장
		this->OriginalLocation = GetActorLocation();
	
		// 단일 대상이면 타켓 앞으로 뛰어가게 한다. (타겟 앞쪽으로 약간 떨어진 위치)
		if (Targets.Num() == 1)
		{
			FVector Direction = (Targets[0]->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			this->TargetLocation = Targets[0]->GetActorLocation() - Direction * 100.0f;
		
			if (IsValid(this->MontageRun) && IsValid(this->AnimInstance))
			{
				this->AnimInstance->Montage_Play(this->MontageRun,1.0f);
			}
		}
		
	} else // 마법계 스킬 시전
	{
		if (IsValid(this->MontageSuicide) && IsValid(this->AnimInstance))
		{
			float MontageLength = this->AnimInstance->Montage_Play(
				this->MontageSuicide,1.0f, EMontagePlayReturnType::MontageLength,
				0.0f, true);

			FTimerHandle Th;
			FTimerHandle Th2;

			GetWorldTimerManager().SetTimer(Th2, [this]()
			{
				if (IsValid(this->CurrentSkill.PreEffect))
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						GetWorld(),
						this->CurrentSkill.PreEffect,
						this->GetActorLocation(),
						this->GetActorRotation(),
						FVector(1.0f),
						true
					);
				}
			}, MontageLength-0.7f, false);
			
			GetWorldTimerManager().SetTimer(Th, [this]()
			{
				this->ExecSkillMontages();
			},MontageLength, false);
		}
	}
}

void ALife::Defend()
{
	if (IsValid(this->MontageDefend))
	{
		USkeletalMeshComponent* MeshComp = GetMesh();
		if (IsValid(MeshComp) && MeshComp->GetAnimInstance())
		{
			bIsDefend = true;
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
		bIsDefend = false;
	}
}

void ALife::ShowHpBar()
{
	if (HpBarWidgetComponent)
	{
		HpBarWidgetComponent->SetVisibility(true);
	}
}

void ALife::HideHpBar()
{
	if (HpBarWidgetComponent)
	{
		HpBarWidgetComponent->SetVisibility(false);
	}
}

void ALife::MakeDown()
{
	this->AnimInstance->Montage_Play(this->MontageDown,1.0f);
	this->bIsDown = true;
}

void ALife::RestoreDown()
{
	this->AnimInstance->Montage_Stop(0.5f, this->MontageDown);
	this->bIsDown = false;
}


