// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./Component/LifeEquipComponent.h"
#include "./Component/LifeStatComponent.h"
#include "Components/WidgetComponent.h"
#include "Data/StructSkill.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "UI/Battle/Widget3DLifeHealthBar.h"
#include "Life.generated.h"

class ABattleGameMode;

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

	// HP Bar Widget Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HpBarWidgetComponent = nullptr;

	// HP Bar Widget Class (블루프린트에서 할당)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UWidget3DLifeHealthBar> HpBarWidgetClass;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontageHit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontageDefendHit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontageDead;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontageSuicide;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontageDown;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Battle Animations")
	UAnimMontage* MontageWin;

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

	UPROPERTY(BlueprintReadWrite)
	bool bIsDefend = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDown = false;
	
	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	FRowSkill CurrentSkill;

	UPROPERTY()
	FRowSkillRecord CurrentSkillRecord;

	UPROPERTY()
	FString TempCode = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Skills")
	TArray<FRowSkillRecord> SkillRecords;

	UPROPERTY()
	ABattleGameMode* MyGameMode = nullptr;

	UFUNCTION()
	void LaunchProjectileToTarget(ALife* Life);

	bool UpdateProjectileMovement(UNiagaraComponent* Projectile, const FVector& StartLocation, const FVector& EndLocation, float Duration, TSharedPtr<float> ElapsedTime);

	void CleanupProjectile(UNiagaraComponent* Projectile, TSharedPtr<FTimerHandle> TimerHandle);
	

public:
	void OnSkillMontageEnded(UAnimMontage* AnimMontage, bool bArg);
	
	void ExecSkillMontages();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ExecSkill(TArray<ALife*> Targets, FRowSkill Skill, FRowSkillRecord SkillRecord);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULifeEquipComponent* LifeEquipComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULifeStatComponent* LifeStatComponent;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Defend();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void ResetAnim();

	UFUNCTION(BlueprintCallable)
	void ShowHpBar();

	UFUNCTION(BlueprintCallable)
	void HideHpBar();

	UFUNCTION(BlueprintCallable)
	void MakeDown();

	UFUNCTION(BlueprintCallable)
	void RestoreDown();

	UFUNCTION()
	bool GetIsDown() const { return this->bIsDown; };

	UFUNCTION(BlueprintCallable)
	bool GetIsDefend() const { return bIsDefend; }

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetMontageWin() const { return MontageWin; }

	void SetTmpCode(FString NewCode) { TempCode = NewCode; }

	UFUNCTION(BlueprintCallable)
	FString GetTmpCode() const { return TempCode; }

	UPROPERTY()
	bool AmIHuman = false;
};
