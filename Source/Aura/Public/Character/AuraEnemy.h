// Copyright 阿香不爱玩UE

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
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

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	
	//敌人等级通过服务器设置不需要复制
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

};
