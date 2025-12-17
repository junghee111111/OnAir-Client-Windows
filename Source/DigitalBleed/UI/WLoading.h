// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Slate/SLoading.h"
#include "WLoading.generated.h"
/**
 * 
 */
UCLASS()
class DIGITALBLEED_API UWLoading : public UWidget
{
	GENERATED_BODY()
public:
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
	FSlateBrush RotatingBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
	FSlateBrush BackgroundBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation")
	float RotationSpeed = 0.2f;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	TSharedPtr<SLoading> LoadingSlate;
};
