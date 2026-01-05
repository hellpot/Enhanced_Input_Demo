// MyCharacter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ENHANCED_INPUT_DEMO_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 移动输入回调
	void Move(const FInputActionValue& Value);
	
	// 视角输入回调
	void Look(const FInputActionValue& Value);

public:
	// 摄像机组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	// 输入配置 - 在蓝图或编辑器中指定
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
};
