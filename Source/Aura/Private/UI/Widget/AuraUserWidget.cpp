// Copyright 阿香不爱玩UE


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	//在控制器对象被设置后调用 回调函数
	WidgetControllerSet();
	
}
