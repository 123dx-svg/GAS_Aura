// Copyright 阿香不爱玩UE


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement=true;
	GetCharacterMovement()->RotationRate=FRotator(0.0f,400.f,0.0f);
	//topdown游戏特性
	GetCharacterMovement()->bConstrainToPlane=true;
	GetCharacterMovement()->bSnapToPlaneAtStart=true;

	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//此处是给服务端用的 初始化ActorInfo
	InitAbilityActorInfo();
	UE_LOG(LogTemp, Log, TEXT("服务器初始化ActorInfo"))
	//添加玩家初始化能力
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//此处是客户端用，初始化ActorInfo
	InitAbilityActorInfo();
	UE_LOG(LogTemp, Log, TEXT("客户端初始化ActorInfo"))
}

int32 AAuraCharacter::GetPlayerLevel()
{
	const AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//此处的逻辑拥有者是PS，而实际拥有者是玩家
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);

	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet=AuraPlayerState->GetAttributeSet();

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			//ASC初始化完成，需要在客户端和服务器都要初始化结束
			AuraHUD->InitOverlay(AuraPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}

	//GE初始化玩家属性
	InitializeDefaultAttributes();
}
