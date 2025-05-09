// Copyright 阿香不爱玩UE


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	//本地玩家调用，服务端不调用
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
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
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//CASE 2
			ThisActor->HighlightActor();
		}
		else
		{
			//CASE 1
		}
	}
	else//LastActor有效
	{
		if (ThisActor == nullptr)
		{
			//CASE 3
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				//CASE 4
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//CASE 5
			}
			
		}
	}
	
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
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
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
