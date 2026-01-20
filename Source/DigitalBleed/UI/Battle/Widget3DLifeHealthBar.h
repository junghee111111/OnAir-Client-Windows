// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Life.h"
#include "Blueprint/UserWidget.h"
#include "Widget3DLifeHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWidget3DLifeHealthBar : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 DisplayHp;

	UPROPERTY(BlueprintReadOnly)
	int32 TargetHp;

	UPROPERTY(BlueprintReadOnly)
	ALife* TargetLife = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	void SetTargetLife(ALife* NewTargetLife, int32 NewDisplayHp);

	UFUNCTION(BlueprintCallable)
	void SetTargetHp(int32 NewTargetHp);
	
};
