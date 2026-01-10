// TopDownPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TopDownPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ENHANCED_INPUT_DEMO_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownPlayerController();

	// Enhanced Input 配置
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ClickAction;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	// 鼠标左键点击回调
	void OnClick(const FInputActionValue& Value);
};
