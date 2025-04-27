// Copyright 阿香不爱玩UE


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		// 精确匹配
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp,Error, TEXT("Can not find Info for AttributeTag[%s] on AttributeInfo[%s]"), *AttributeTag.ToString(),*GetNameSafe(this));
	}
	return FAuraAttributeInfo();
}
