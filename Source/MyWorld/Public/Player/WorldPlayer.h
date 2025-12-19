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
class UStaticMeshComponent;
class USkeletalMeshComponent;
class AInteractItem;
class AWateringcan;

UCLASS()
class MYWORLD_API AWorldPlayer  : public ACharacter
{
	GENERATED_BODY()

public:
	AWorldPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void CallTouchingBP(AActor* ActorOverlap);
	void CallWateringBP(AActor* ActorOverlap);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Properties|Widget")
	void SetAmountOfElementsToWidget(int32 Rocks, int32 Flowers, int32 Trees, int32 Lights);

	virtual void SetAmountOfElementsToWidget_Implementation(int32 Rocks, int32 Flowers, int32 Trees, int32 Lights);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Properties|Widget")
	void SetAmountOfElementsToPlayer(int32 Rocks, int32 Flowers, int32 Trees, int32 Lights);

	virtual void SetAmountOfElementsToPlayer_Implementation(int32 Rocks, int32 Flowers, int32 Trees, int32 Lights);


	/*Funcion for update data own spawnerBP*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Properties|References")
	void UpdateSpawnerDataBP();
	virtual void UpdateSpawnerDataBP_Implementation();


	/* References */
	UPROPERTY(EditAnywhere, Category = "Player Properties|References")
	AActor* OverlappingActor;
	UPROPERTY(EditAnywhere, Category = "Player Properties|References")
	ACharacter* PlayerCharacter;
	/* References */

	/* References Interactable Objects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Properties|References")
	AInteractItem* OverlappingInteractItem = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Properties|References")
	AInteractItem* ItemInHand = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Properties|References")
	AInteractItem* EquipableObject = nullptr;
	/* References Interactable Objects */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Properties|References")
	float IsInHand = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Properties|References")
	bool bIsInHand = false;


	/* Amount of Objects*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 AmountOfRocks = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 AmountOfFlowers = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 AmountOfTrees = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 AmountOfLights = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 CurrentAmountOfRocks = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 CurrentAmountOfFlowers = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 CurrentAmountOfTrees = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 CurrentAmountOfLights = 0;


	// Camera Variables
	UPROPERTY(EditAnywhere, Category = "Player Properties|Camera")
	float LookSensitivity = 1.0f;



protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player Actions")
	void playMontageAnim(UAnimMontage* MontageToPlay);

	void playMontageAnim_Implementation(UAnimMontage* MontageToPlay);
;


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

	// Control de aceleración correr caminar
	UPROPERTY(EditAnywhere, Category = "Player Properties|Movement")
	float SpeedInterpRate;
	bool bIsRunning = false;

	/* Movement Variables */
	UPROPERTY(EditDefaultsOnly, Category = "Player Properties|Movement")
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Player Properties|Movement")
	float RunSpeed;
	UPROPERTY(VisibleAnywhere, Category = "Player Properties|Movement")
	float CurrentSpeed;

	/* Camera Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleOverlap;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* ClothesMesh;

	/* Camera Variables */


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Properties|Tick")
	float DeltaSeconds;






private:
	void DebugMessage(int32 Key, FString Message, FColor Color = FColor::Green, float Duration = 2.0f);

};