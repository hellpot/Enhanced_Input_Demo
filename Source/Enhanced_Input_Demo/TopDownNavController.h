// TopDownNavController.h
// 使用 AIController::MoveToLocation 的俯视角控制器
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TopDownNavController.generated.h"

class UInputMappingContext;
class UInputAction;
class ATopDownNavCharacter;

UCLASS()
class ENHANCED_INPUT_DEMO_API ATopDownNavController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownNavController();

	// Enhanced Input 配置
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ClickAction;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// 鼠标左键点击回调
	void OnClick(const FInputActionValue& Value);

private:
	// 获取控制的 NavCharacter
	ATopDownNavCharacter* GetNavCharacter() const;
};
