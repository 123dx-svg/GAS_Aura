﻿// Copyright 阿香不爱玩UE


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	//服务端发射
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;


	//不依赖Aura角色通过接口获取发射位置
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (TargetLocation-SocketLocation).Rotation();
		//发射物保持水平
		Rotation.Pitch = 0;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		AAuraProjectile* Projectile =  GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,SpawnTransform,GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//todo:需要提供Gameplay Effect Spec 计算造成伤害


		
		Projectile->FinishSpawning(SpawnTransform);
	
	}
}
