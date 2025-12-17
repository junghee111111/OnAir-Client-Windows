// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WLoading.h"

#define LOCTEXT_NAMESPACE "UMG"

void UWLoading::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	LoadingSlate->SetRotatingBrush(&RotatingBrush);
	LoadingSlate->SetBackgroundBrush(&BackgroundBrush);
	LoadingSlate->SetRotationSpeed(RotationSpeed);
}

void UWLoading::ReleaseSlateResources(bool bReleaseChildren)
{
	LoadingSlate.Reset();
}

TSharedRef<SWidget> UWLoading::RebuildWidget()
{
	LoadingSlate = SNew(SLoading)
	.RotatingBrush(&RotatingBrush)
	.BackgroundBrush(&BackgroundBrush)
	.RotationSpeed(RotationSpeed);
	return LoadingSlate.ToSharedRef();
}


#if WITH_EDITOR
const FText UWLoading::GetPaletteCategory()
{
	return LOCTEXT("CustomPaletteCategory", "Loading Slate");
}
#endif
