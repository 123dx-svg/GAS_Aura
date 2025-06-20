// Copyright 阿香不爱玩UE


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{

	for (const FAuraInputAction& AbilityInputAction : AbilityInputActions)
	{
		if (AbilityInputAction.InputAction && AbilityInputAction.InputTag == InputTag)
		{
			return AbilityInputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find input action for ability input tag %s on InputConfig %s. Check your mapping configuration."),
			*InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
