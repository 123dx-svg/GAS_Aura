// Copyright 阿香不爱玩UE

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacterBase();
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//方便起见也写个获取Set的方法 玩家身上获取不到
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	//初始化属性
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffect,float Level) const;
	void InitializeDefaultAttributes() const;
	
};
