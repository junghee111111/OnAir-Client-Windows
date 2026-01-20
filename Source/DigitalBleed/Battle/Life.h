// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./Component/LifeEquipComponent.h"
#include "./Component/LifeStatComponent.h"
#include "Data/StructSkill.h"
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontagePhysAttack1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontagePhysAttack2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontagePhysAttack3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontageRun;

	UPROPERTY()
	UAnimInstance* AnimInstance = nullptr;

	UPROPERTY()
	FVector OriginalLocation;

	UPROPERTY()
	bool bIsMovingToTarget = false;

	UPROPERTY()
	bool bIsGoingOriginalPos = false;

	UPROPERTY()
	bool bReadyForExecuteSkill = false;

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	FRowSkill CurrentSkill;

	UPROPERTY()
	FRowSkillRecord CurrentSkillRecord;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Skills")
	TArray<FRowSkillRecord> SkillRecords;
	

public:
	void OnSkillMontageEnded(UAnimMontage* AnimMontage, bool bArg);
	void ExecSkillMontages();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ExecSkill(ALife* TargetLife, FRowSkill Skill, FRowSkillRecord SkillRecord);

	UPROPERTY(EditAnywhere)
	ULifeEquipComponent* LifeEquipComponent;
	
	UPROPERTY(EditAnywhere)
	ULifeStatComponent* LifeStatComponent;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Defend();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void ResetAnim();
};
