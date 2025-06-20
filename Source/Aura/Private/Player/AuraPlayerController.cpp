// Copyright 阿香不爱玩UE


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	//本地玩家调用，服务端不调用
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	//自动跑
	AutoRun();

}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		//此处为沿着样条线跑
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination =FVector::Distance(LocationOnSpline,CachedDestination);
		if (DistanceToDestination<=AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}
void AAuraPlayerController::CursorTrace()
{

	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit)return;
	LastActor=ThisActor;
	ThisActor=Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	 * 鼠标发出的射线检测的几种情况
	 * 1、ThisActor和LastActor都是空 什么都不做
	 * 2、Last是空，This有效  高亮this
	 * 3、Last有效，This无效  取消高亮Last
	 * 4、两者都有效果，但是This不等于Last  取消高亮Last 高亮this
	 * 5、两者都有效，但两者相同  什么都不做
	 */
	// if (LastActor == nullptr)
	// {
	// 	if (ThisActor != nullptr)
	// 	{
	// 		//CASE 2
	// 		ThisActor->HighlightActor();
	// 	}
	// 	else
	// 	{
	// 		//CASE 1
	// 	}
	// }
	// else//LastActor有效
	// {
	// 	if (ThisActor == nullptr)
	// 	{
	// 		//CASE 3
	// 		LastActor->UnHighlightActor();
	// 	}
	// 	else
	// 	{
	// 		if (LastActor != ThisActor)
	// 		{
	// 			//CASE 4
	// 			LastActor->UnHighlightActor();
	// 			ThisActor->HighlightActor();
	// 		}
	// 		else
	// 		{
	// 			//CASE 5
	// 		}
	// 		
	// 	}
	// }
	//简化
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
	
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//判断有没有点到敌人身上
		bTargeting = ThisActor?true:false;
		bAutoRunning = false;
	}
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	
	if (!bTargeting&&!bShiftPressed)
	{
		const APawn* ControlledPawn = GetPawn();
		//短按
		if (FollowTime<= ShortPressThreshold)
		{
			//从导航路径中获取路径点并规避障碍
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(),CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointsLoc: NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointsLoc,ESplineCoordinateSpace::World);
					//DrawDebugSphere(GetWorld(),PointsLoc,10,12,FColor::Green,false,5.f);
				}
				CachedDestination =  NavPath->PathPoints.Last();
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
	
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//首先看是否是左键按下或者长期点到敌人身上，这种情况应该是其他技能
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	if (bTargeting||bShiftPressed)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

	
		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;
		
		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
	 	AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}



void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraInputContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		//多人模式其他玩家就不回找这个Subsystem
		Subsystem->AddMappingContext(AuraInputContext,0);
	}
	

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::Default;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//如果转换失败就会触发断言
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&AAuraPlayerController::ShiftReleased);
	
	

	//通过模版函数来绑定AbilityInputAction
	AuraInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0,Rotation.Yaw,0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
		
	}
}
