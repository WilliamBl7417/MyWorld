#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <EnhancedInputLibrary.h>
#include "WorldPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class ACharacter;
class UCapsuleComponent;

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

	void RunStart(const FInputActionValue& Value);
	void RunStop(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent)
	void RunStartBP();
	void RunStartBP_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void RunStopBP();
	void RunStopBP_Implementation();


	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) ;

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CallTouchingBP(AActor* ActorOverlap);


	/* Input */
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

	/* References */
	ACharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* OverlappingActor;


	/* Camera Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleOverlap;

	/* Variables */

	/* Camera Variables */
	UPROPERTY(EditAnywhere, Category = "Player Properties|Camera")
	float LookSensitivity = 1.0f;

	/* Movement Variables */
	UPROPERTY(EditAnywhere, Category = "Player Properties|Movement")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, Category = "Player Properties|Movement")
	float RunSpeed;
	UPROPERTY(VisibleAnywhere, Category = "Player Properties|Movement")
	float CurrentSpeed;

	// Control de aceleración
	UPROPERTY(EditAnywhere, Category = "Player Properties|Movement")
	float SpeedInterpRate; 

	bool bIsRunning = false;


protected:

	virtual void BeginPlay() override;

private:
	void DebugMessage(int32 Key,FString Message, FColor Color = FColor::Green, float Duration = 2.0f);

};
