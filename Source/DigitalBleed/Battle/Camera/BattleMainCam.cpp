// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMainCam.h"


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

void ABattleMainCam::SeePlayerBack()
{
}

void ABattleMainCam::SeeEnemyBackToPlayer()
{
}

void ABattleMainCam::SeePlayerBackToEnemy()
{
}

void ABattleMainCam::StartRotation()
{
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
}

