// TopDownPlayerController.cpp
#include "TopDownPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	// 加载 Enhanced Input 资源
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCFinder(
		TEXT("/Game/topdown/IMC_TopDown.IMC_TopDown"));
	if (IMCFinder.Succeeded())
	{
		DefaultMappingContext = IMCFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IAFinder(
		TEXT("/Game/topdown/IA_Click.IA_Click"));
	if (IAFinder.Succeeded())
	{
		ClickAction = IAFinder.Object;
	}
}

void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 注册 Enhanced Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATopDownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 绑定 Enhanced Input Action
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ClickAction)
		{
			EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &ATopDownPlayerController::OnClick);
		}
	}
}

void ATopDownPlayerController::OnClick(const FInputActionValue& Value)
{
	FHitResult HitResult;
	
	// 获取鼠标点击位置的射线检测结果
	if (GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		// 使用 Simple Move to Location 移动到点击位置
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitResult.Location);
	}
}
