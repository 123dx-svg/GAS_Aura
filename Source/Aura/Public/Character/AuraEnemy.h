// Copyright 阿香不爱玩UE

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	
	/*敌人接口Begin*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/*敌人接口End*/

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	


};
