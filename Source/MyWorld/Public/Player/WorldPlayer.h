#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <EnhancedInputLibrary.h>
#include "WorldPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MYWORLD_API AWorldPlayer : public ACharacter
{
	GENERATED_BODY()

public:

	AWorldPlayer();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Inputs // IMC Implementation*/

	void MoveEvent(const FInputActionValue& Value);
	void LookEvent(const FInputActionValue& Value);
	void InteractEvent(const FInputActionValue& Value);
	void JumpEvent(const FInputActionValue& Value);
	void RunEvent(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RunAction;


	/* Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Player Properties|Camera")
	float LookSensitivity = 1.0f;



protected:

	virtual void BeginPlay() override;

public:	

};
