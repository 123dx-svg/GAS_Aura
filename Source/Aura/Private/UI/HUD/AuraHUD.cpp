// Copyright 阿香不爱玩UE


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	Widget->AddToViewport();
}
