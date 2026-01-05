// MyCharacter.cpp
#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 创建 Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true; // 跟随控制器旋转

	// 创建摄像机
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// 角色不跟随控制器旋转，由移动方向决定
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 角色朝移动方向转身
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 注册输入映射上下文
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 绑定增强输入
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		}
	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	// 获取输入的 2D 向量
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 获取控制器的 Yaw 旋转（水平朝向）
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 计算前方和右方方向
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 添加移动输入
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	// 获取鼠标输入的 2D 向量
	FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 添加 Yaw 输入（左右转）
		AddControllerYawInput(LookVector.X);
		
		// 添加 Pitch 输入（上下看，取反）
		AddControllerPitchInput(-LookVector.Y);
	}
}
