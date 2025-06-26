// Copyright 阿香不爱玩UE


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{

	return CharacterClassInformation.FindChecked(CharacterClass);
}
