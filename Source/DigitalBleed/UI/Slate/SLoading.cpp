// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "SLoading.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLoading::Construct(const FArguments& InArgs)
{
	RotatingBrush = InArgs._RotatingBrush;
	BackgroundBrush = InArgs._BackgroundBrush;
	Sequence = FCurveSequence();

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		  .HAlign(HAlign_Center)
		  .VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(BackgroundBrush)
		]
		+ SOverlay::Slot()
		  .HAlign(HAlign_Center)
		  .VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(RotatingBrush)
		]
	];

	CurveHandle = Sequence.AddCurve(0.f, 60.f);
	Sequence.Play(AsShared(), true, 0.f, false);
}

int32 SLoading::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	const auto RotationTransform = FSlateRenderTransform(
	FQuat2D(CurveHandle.GetLerp() * 360.f * RotationSpeed)
);

	const FGeometry RotatingImageGeo = AllottedGeometry.MakeChild(
		RotatingBrush->ImageSize,
		FSlateLayoutTransform(),
		RotationTransform,
		FVector2D(0.5f, 0.5f)
		);

	const FGeometry BackgroundImageGeo = AllottedGeometry.MakeChild(
		BackgroundBrush->ImageSize,
		FSlateLayoutTransform(),
		FSlateRenderTransform(),
		FVector2D(0.5f, 0.5f)
		);
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		BackgroundImageGeo.ToPaintGeometry(),
		BackgroundBrush,
		ESlateDrawEffect::None,
		InWidgetStyle.GetColorAndOpacityTint() * BackgroundBrush->GetTint(InWidgetStyle)
		);

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		RotatingImageGeo.ToPaintGeometry(),
		RotatingBrush,
		ESlateDrawEffect::None,
		InWidgetStyle.GetColorAndOpacityTint() * RotatingBrush->GetTint(InWidgetStyle)
		);

	return LayerId;
}

void SLoading::SetRotatingBrush(const FSlateBrush* Brush)
{
	RotatingBrush = Brush;
}

void SLoading::SetBackgroundBrush(const FSlateBrush* Brush)
{
	BackgroundBrush = Brush;
}

float SLoading::GetRotationSpeed() const
{
	return RotationSpeed;
}

void SLoading::SetRotationSpeed(float InRotationSpeed)
{
	RotationSpeed = InRotationSpeed;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
