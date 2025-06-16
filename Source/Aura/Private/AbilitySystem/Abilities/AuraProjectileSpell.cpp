// Copyright 阿香不爱玩UE


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Yellow,
		FString(TEXT("ActivateAbility C++"))
	);
}
