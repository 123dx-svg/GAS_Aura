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

	/*战斗接口Begin*/
	virtual int32 GetPlayerLevel() override;
	 /*战斗接口End*/

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	int32 Level = 1;

};
