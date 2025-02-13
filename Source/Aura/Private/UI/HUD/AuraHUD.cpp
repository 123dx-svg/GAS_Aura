// Copyright 阿香不爱玩UE


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{

	checkf(OverlayWidgetClass,TEXT("OverlayWidgetClass为空 请在BP_AuraHUD将此类赋值"));
	checkf(OverlayWidgetControllerClass,TEXT("OverlayWidgetControllerClass为空 请在BP_AuraHUD将此类赋值"));
	
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WCParams(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController =GetOverlayWidgetController(WCParams);

	OverlayWidget->SetWidgetController(WidgetController);
	
	Widget->AddToViewport();
}

