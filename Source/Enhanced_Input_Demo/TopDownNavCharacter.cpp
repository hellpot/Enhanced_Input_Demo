// TopDownNavCharacter.cpp
#include "TopDownNavCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

ATopDownNavCharacter::ATopDownNavCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 创建 Spring Arm（俯视角配置）
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->SetRelativeRotation(FRotator(-55.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	// 创建摄像机
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// 角色不跟随控制器旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 角色朝移动方向转身
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	NavAIController = nullptr;
}

void ATopDownNavCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 生成独立的 AI 控制器用于导航
	SpawnNavAIController();
}

void ATopDownNavCharacter::SpawnNavAIController()
{
	// 生成 AI 控制器（不 Possess，只用于导航计算）
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	NavAIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnParams);

	if (NavAIController)
	{
		// 设置 Pawn 但不 Possess（保持 PlayerController 控制）
		NavAIController->SetPawn(this);

		// 绑定移动完成回调
		if (UPathFollowingComponent* PathComp = NavAIController->GetPathFollowingComponent())
		{
			PathComp->OnRequestFinished.AddUObject(this, &ATopDownNavCharacter::HandleMoveCompleted);
		}

		UE_LOG(LogTemp, Warning, TEXT("NavAIController 创建成功"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NavAIController 创建失败"));
	}
}

void ATopDownNavCharacter::MoveToLocation(const FVector& Destination)
{
	if (NavAIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("开始移动到: %s"), *Destination.ToString());
		EPathFollowingRequestResult::Type Result = NavAIController->MoveToLocation(Destination, AcceptanceRadius, true, true, false, true);
		
		switch (Result)
		{
		case EPathFollowingRequestResult::Failed:
			UE_LOG(LogTemp, Error, TEXT("MoveToLocation 失败：路径查找失败（检查 NavMesh）"));
			break;
		case EPathFollowingRequestResult::AlreadyAtGoal:
			UE_LOG(LogTemp, Warning, TEXT("MoveToLocation：已在目标位置"));
			break;
		case EPathFollowingRequestResult::RequestSuccessful:
			UE_LOG(LogTemp, Warning, TEXT("MoveToLocation：请求成功，开始移动"));
			break;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NavAIController 无效"));
	}
}

void ATopDownNavCharacter::StopMovement()
{
	if (NavAIController)
	{
		NavAIController->StopMovement();
	}
}

void ATopDownNavCharacter::HandleMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	bool bSuccess = Result.IsSuccess();
	OnMoveCompleted(bSuccess);
}

void ATopDownNavCharacter::OnMoveCompleted_Implementation(bool bSuccess)
{
	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("到达目标位置"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("移动失败或被中断"));
	}
}
