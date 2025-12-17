// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGITALBLEED_API SLoading : public SCompoundWidget
{
private:
	FCurveSequence Sequence;
	FCurveHandle CurveHandle;

	const FSlateBrush* RotatingBrush;
	const FSlateBrush* BackgroundBrush;
	float RotationSpeed;
	
public:
	SLATE_BEGIN_ARGS(SLoading)
		: _RotatingBrush()
			, _BackgroundBrush()
			, _RotationSpeed(0.2f)
	{
	}

		SLATE_ARGUMENT(const FSlateBrush*, RotatingBrush)
		SLATE_ARGUMENT(const FSlateBrush*, BackgroundBrush)
		SLATE_ARGUMENT(float, RotationSpeed)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	int32 OnPaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void SetRotatingBrush(const FSlateBrush* Brush);
	void SetBackgroundBrush(const FSlateBrush* Brush);
	float GetRotationSpeed() const;
	void SetRotationSpeed(float RotationSpeed);
};
