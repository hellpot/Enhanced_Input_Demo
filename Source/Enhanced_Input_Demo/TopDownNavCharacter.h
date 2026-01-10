// TopDownNavCharacter.h
// 使用 AIController 进行 Nav Mesh 导航的俯视角角色
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "TopDownNavCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AAIController;

UCLASS()
class ENHANCED_INPUT_DEMO_API ATopDownNavCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATopDownNavCharacter();

	// 摄像机组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	// 移动到目标位置（使用 AIController）
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveToLocation(const FVector& Destination);

	// 停止移动
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopMovement();

	// 到达目标的接受半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AcceptanceRadius = 50.0f;

	// 移动完成回调（可在蓝图中重写）
	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void OnMoveCompleted(bool bSuccess);

protected:
	virtual void BeginPlay() override;

	// 内部 AI 控制器（用于导航）
	UPROPERTY()
	AAIController* NavAIController;

private:
	// 生成 AI 控制器
	void SpawnNavAIController();

	// 移动完成回调处理
	void HandleMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
};
