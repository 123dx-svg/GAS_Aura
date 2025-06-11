// Copyright 阿香不爱玩UE

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"




UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAuraInputComponent();
	template<typename UserClass,typename PressedFuncType,typename ReleaseFuncType,typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig,UserClass* Object,PressedFuncType PressFunc,ReleaseFuncType ReleaseFunc,HeldFuncType HeldFunc);
	


};

template <typename UserClass, typename PressedFuncType, typename ReleaseFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressFunc, ReleaseFuncType ReleaseFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FAuraInputAction& InputAction : InputConfig->AbilityInputActions)
	{
		if (InputAction.InputAction&&InputAction.InputTag.IsValid())
		{
			if (PressFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Started, Object, PressFunc, InputAction.InputTag);
			}
			if (ReleaseFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Completed, Object, ReleaseFunc, InputAction.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(InputAction.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputAction.InputTag);
			}
		}
	}
}
