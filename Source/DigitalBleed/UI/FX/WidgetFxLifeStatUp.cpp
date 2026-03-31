// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "WidgetFxLifeStatUp.h"

void UWidgetFxLifeStatUp::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidgetFxLifeStatUp::PlayAnimInit()
{
	this->PlayAnimation(this->AnimInit, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, true);
}
