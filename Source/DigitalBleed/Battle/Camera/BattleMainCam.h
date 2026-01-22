// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "BattleMainCam.generated.h"

UCLASS()
class DIGITALBLEED_API ABattleMainCam : public AActor
{
	GENERATED_BODY()

	bool IsRotating = false;
	FVector CenterPos = FVector(0,0,0);

	

public:
	// Sets default values for this actor's properties
	ABattleMainCam();

	UFUNCTION(BlueprintCallable)
	void SeePlayerBack(const FVector PlayerPos);
	void SeePlayerCenterToMargin(FVector PlayerPos);

	UFUNCTION(BlueprintCallable)
	void SeeEnemyBackToPlayer(FVector PlayerPos, FVector EnemyPos);

	UFUNCTION(BlueprintCallable)
	void GoTowardsTarget(FVector PlayerPos, FVector EnemyPos);

	UFUNCTION(BlueprintCallable)
	void SeePlayerBackToEnemy(FVector PlayerPos, FVector EnemyPos);

	UFUNCTION(BlueprintCallable)
	void StartRotation();
	void StartRotationWithArmLength(float ArmLength);

	UFUNCTION(BlueprintCallable)
	void StopRotation();

	UFUNCTION(BlueprintCallable)
	void SetPitch(float Pitch);

	UFUNCTION(BlueprintCallable)
	void SeePlayerBackToEnemy_Angle1(const FVector& SkillExecutionerPos, const FVector& TargetPos);

	UFUNCTION(BlueprintCallable)
	void SeePlayerBackToEnemy_Angle2(const FVector& SkillExecutionerPos, const FVector& TargetPos);

	UFUNCTION(BlueprintCallable)
	void SeePlayerBackToEnemy_Angle3(const FVector& SkillExecutionerPos, const FVector& TargetPos);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
	UCameraComponent* MainCam = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent = nullptr;

	UPROPERTY()
	FVector TargetLocation = FVector(0,0,0);

	UPROPERTY()
	FRotator TargetRotation = FRotator(0,0,0);

	UPROPERTY()
	float TargetArmLength = 1000;

	UPROPERTY()
	bool bIsMovingToTarget = false;

	UPROPERTY()
	bool bIsForce = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
