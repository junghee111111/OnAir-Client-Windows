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

	
	this->SpringArm->TargetArmLength = 1000.0f;
	this->SpringArm->bUsePawnControlRotation = false;
	this->SpringArm->bDoCollisionTest = false;
	
}

void ABattleMainCam::SeePlayerBack(FVector Pos)
{
	this->SpringArm->TargetArmLength = 300.0f;
	TargetLocation = Pos;

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Pos, FVector(0,0,0));
	TargetRotation = Rot;

	bIsMovingToTarget = true;
}

void ABattleMainCam::SeeEnemyBackToPlayer()
{
}

void ABattleMainCam::SeePlayerBackToEnemy()
{
}

void ABattleMainCam::StartRotation()
{
	this->SpringArm->TargetArmLength = 1000.0f;
	this->bIsMovingToTarget = false;
	this->IsRotating = true;
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
		
		// 목표에 거의 도달하면 정확히 설정하고 중지
		if (FVector::Dist(GetActorLocation(), TargetLocation) < 1.0f)
		{
			SetActorLocation(TargetLocation);
			SetActorRotation(TargetRotation);
			bIsMovingToTarget = false;
		}
	}
}

