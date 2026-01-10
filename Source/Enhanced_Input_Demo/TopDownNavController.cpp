// TopDownNavController.cpp
#include "TopDownNavController.h"
#include "TopDownNavCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

ATopDownNavController::ATopDownNavController()
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

void ATopDownNavController::BeginPlay()
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

void ATopDownNavController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 绑定 Enhanced Input Action
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ClickAction)
		{
			EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &ATopDownNavController::OnClick);
		}
	}
}

void ATopDownNavController::OnClick(const FInputActionValue& Value)
{
	FHitResult HitResult;
	
	if (GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		UE_LOG(LogTemp, Warning, TEXT("点击位置: %s"), *HitResult.Location.ToString());
		
		if (ATopDownNavCharacter* NavChar = GetNavCharacter())
		{
			UE_LOG(LogTemp, Warning, TEXT("NavCharacter 有效，调用 MoveToLocation"));
			NavChar->MoveToLocation(HitResult.Location);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NavCharacter 无效！Pawn 类型错误"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("点击未命中任何物体"));
	}
}

ATopDownNavCharacter* ATopDownNavController::GetNavCharacter() const
{
	return Cast<ATopDownNavCharacter>(GetPawn());
}
