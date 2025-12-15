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
	void SeePlayerBack();

	UFUNCTION(BlueprintCallable)
	void SeeEnemyBackToPlayer();

	UFUNCTION(BlueprintCallable)
	void SeePlayerBackToEnemy();

	UFUNCTION(BlueprintCallable)
	void StartRotation();

	UFUNCTION(BlueprintCallable)
	void StopRotation();

	UFUNCTION(BlueprintCallable)
	void SetPitch(float Pitch);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
	UCameraComponent* MainCam = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent = nullptr;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
