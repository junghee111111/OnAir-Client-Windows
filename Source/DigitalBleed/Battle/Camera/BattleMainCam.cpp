// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMainCam.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABattleMainCam::ABattleMainCam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	this->MainCam = CreateDefaultSubobject<UCameraComponent>("MainCam");
	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>("MainCamSpringArm");

	this->CapsuleComponent->SetupAttachment(this->RootComponent);
	this->SpringArm->SetupAttachment(this->CapsuleComponent);
	this->MainCam->SetupAttachment(this->SpringArm);

	
	this->TargetArmLength = 1000.0f;
	this->SpringArm->bUsePawnControlRotation = false;
	this->SpringArm->bDoCollisionTest = false;
	
}

void ABattleMainCam::SeePlayerBack(const FVector PlayerPos)
{
	this->TargetArmLength = 180.0f;
	TargetLocation = PlayerPos;

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(PlayerPos, FVector(0,0,-100));
	TargetRotation = Rot;

	bIsMovingToTarget = true;
}

void ABattleMainCam::SeeEnemyBackToPlayer()
{
}

void ABattleMainCam::GoTowardsTarget(FVector PlayerPos, FVector EnemyPos)
{
	this->TargetArmLength = 150.0f;
	TargetLocation = PlayerPos + FVector(0,0,70);

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(PlayerPos, EnemyPos);
	TargetRotation = Rot;
	bIsMovingToTarget = true;
}

void ABattleMainCam::SeePlayerBackToEnemy(FVector PlayerPos, FVector EnemyPos)
{
	this->TargetArmLength = 150.0f;
	TargetLocation = PlayerPos + FVector(0,0,30);

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(PlayerPos, EnemyPos-FVector(0,0,50));
	Rot.Yaw += 15.0f; // 시계방향으로 15도정도 뺀다.
	
	TargetRotation = Rot;
	bIsMovingToTarget = true;
}

void ABattleMainCam::StartRotation()
{
	this->SpringArm->TargetArmLength = 1000.0f;
	this->bIsMovingToTarget = false;
	this->IsRotating = true;
}

void ABattleMainCam::SeePlayerBackToEnemy_Angle1(const FVector& SkillExecutionerPos, const FVector& TargetPos)
{
	this->bIsForce = true;
	this->TargetArmLength = 250.0f;
	TargetLocation = SkillExecutionerPos - FVector(0,0,30);

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(SkillExecutionerPos-FVector(0,0,10), TargetPos+FVector(0,0,10));
	Rot.Yaw += 32.0f; // 시계방향으로 15도정도 뺀다.
	
	TargetRotation = Rot;
	bIsMovingToTarget = true;
}

void ABattleMainCam::SeePlayerBackToEnemy_Angle2(const FVector& SkillExecutionerPos, const FVector& TargetPos)
{
	this->bIsForce = true;
	this->TargetArmLength = 350.0f;
	TargetLocation = SkillExecutionerPos + FVector(0,0,30);

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(SkillExecutionerPos, TargetPos-FVector(0,0,200));
	Rot.Yaw -= 5.0f; // 시계방향으로 15도정도 뺀다.
	
	TargetRotation = Rot;
	bIsMovingToTarget = true;
	this->bIsForce = false;
	// FRotator Rot2 = UKismetMathLibrary::FindLookAtRotation(SkillExecutionerPos, TargetPos+FVector(0,0,200));
	// Rot2.Yaw += 10.0f;
	// TargetRotation = Rot2;
}

void ABattleMainCam::SeePlayerBackToEnemy_Angle3(const FVector& SkillExecutionerPos, const FVector& TargetPos)
{
	this->bIsForce = true;
	this->TargetArmLength = 250.0f;
	TargetLocation = SkillExecutionerPos + FVector(0,0,30);

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(SkillExecutionerPos, TargetPos-FVector(0,0,50));
	Rot.Yaw -= 32.0f; // 시계방향으로 15도정도 뺀다.
	
	TargetRotation = Rot;
	bIsMovingToTarget = true;
}

void ABattleMainCam::StopRotation()
{
	this->IsRotating = false;
}

void ABattleMainCam::SetPitch(float Pitch)
{
	this->SetActorRotation(FRotator(Pitch, this->GetActorRotation().Yaw, this->GetActorRotation().Roll));
}

// Called when the game starts or when spawned
void ABattleMainCam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattleMainCam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->IsRotating == true)
	{
		this->AddActorWorldRotation(
			FRotator(0.0f, 1.0f, 0.0f) * DeltaTime * 3.0f,
			false,
			nullptr,
			ETeleportType::None
		);
	}

	if (bIsMovingToTarget)
	{
		// 부드럽게 위치 보간 (InterpSpeed 값이 클수록 빠름, 보통 5.0f ~ 10.0f)
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, 5.0f);
		SetActorLocation(NewLocation);
		
		// 부드럽게 회전 보간
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.0f);
		SetActorRotation(NewRotation);

		float NewTargetArmLength = FMath::FInterpTo(this->SpringArm->TargetArmLength, TargetArmLength, DeltaTime, 5.0f);
		this->SpringArm->TargetArmLength = NewTargetArmLength;

		bool bIsLocationDone = false;
		bool bIsRotationDone = false;
		
		// 목표에 거의 도달하면 정확히 설정하고 중지
		if ((FVector::Dist(GetActorLocation(), TargetLocation) < 0.01f && FMath::Abs(TargetArmLength-NewTargetArmLength) < 1.0f) || bIsForce)
		{
			SetActorLocation(TargetLocation);
			this->SpringArm->TargetArmLength = TargetArmLength;

			bIsLocationDone = true;
		}

		FRotator CurrentRotation = GetActorRotation();
		float AngleDifference = FMath::Abs((TargetRotation - CurrentRotation).GetNormalized().Yaw);

		if (AngleDifference <= 0.1f || bIsForce)
		{
			SetActorRotation(TargetRotation);
			bIsRotationDone = true;
		}

		if (bIsForce)
		{
			bIsForce = false;
		}

		if (bIsLocationDone && bIsRotationDone || bIsForce)
		{
			bIsMovingToTarget = false;
		}
		
	}
}

