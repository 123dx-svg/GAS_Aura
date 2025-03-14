// Copyright 阿香不爱玩UE

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/*战斗接口Begin*/
	virtual int32 GetPlayerLevel() override;
	/*战斗接口End*/

private:
	virtual  void InitAbilityActorInfo() override;
};
