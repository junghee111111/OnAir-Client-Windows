// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./Component/LifeEquipComponent.h"
#include "./Component/LifeStatComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Life.generated.h"

UCLASS(abstract)
class DIGITALBLEED_API ALife : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALife();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontageDefend;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	ULifeEquipComponent* LifeEquipComponent;
	
	UPROPERTY(EditAnywhere)
	ULifeStatComponent* LifeStatComponent;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Defend();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void ResetAnim();
};
