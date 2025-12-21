// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TouchingInterface.h"
#include "InteractItem.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UBoxComponent;
class UTextRenderComponent;
class AWorldPlayer;
class UPlaySoundAndParticles;
class UUserWidget;


UENUM(BlueprintType)
enum class EBooleanOutputPin : uint8
{
	BO_PinTrue		UMETA(DisplayName = "True"),
	BO_PinFalse		UMETA(DisplayName = "False")
};



UCLASS()
class MYWORLD_API AInteractItem : public AActor, public ITouchingInterface
{
	GENERATED_BODY()

public:


	/*Functions*///
	AInteractItem();

	/* Interface implementation*/
	virtual void TouchingBP_Implementation() override;
	virtual void WateringPlantBP_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Interaction | PlayerRef")
	void SavePlayerRef(AWorldPlayer* PlayerRef);

	UFUNCTION(BlueprintCallable, Category = "Interaction | PlayerRef")
	void CleanPlayerRef();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Interaction | Material")
	void ChangeMaterialInteraction();

	virtual void ChangeMaterialInteraction_Implementation();
	
	/*Internal Functions*/
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;



	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ExpandEnumAsExecs = "BranchResult"))
	void CheckAlreadyChangeMat(int32 Valuetochange ,EBooleanOutputPin& BranchResult, int32& OutValue);

	virtual void CheckAlreadyChangeMat_Implementation(int32 Valuetochange, EBooleanOutputPin& BranchResult, int32& OutValue);
	

	/*Functions*/


	/*Variables*/

	/* Components*/
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category = "Components |")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components |")
	UTextRenderComponent* TextRender;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | TextRender")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | SKMesh")
	USkeletalMesh* SkeletalMeshToShow;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | StaticMesh")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Mesh")
	UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Mesh")
	UPlaySoundAndParticles* ObjectEfects;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components |")
	USceneComponent* StaticMeshAttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | StaticMesh")
	UStaticMeshComponent* ShadowMeshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components | BoxCollition")
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | BoxCollitions")
	FVector CollitionBoxScale = FVector(1.f, 1.f, .2f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | BoxCollition")
	float CollitionBoxScaleMultiplier = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties |")
	int32 ElementIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Material dynamic")
	FName MaterialParameterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Material dynamic")
	bool AlreadyChangeMat = false; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Properties | Const")
	FVector InitialScale;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Interaction")
	bool bImplementTouching = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Interaction")
	bool bImplementWhatering = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Interaction")
	bool bIsInHand = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Attaching")
	FName AttachSocketName;

	/*References*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties | References")
	AWorldPlayer* WorldPlayerRef;


	/*Variables*/

	/*Functions*/

	protected:

	UFUNCTION(BlueprintCallable, Category = "Animation | Rates")
	void SetAnimRatePlay(FVector WorldScale,
		float RPB_Min,
		float RPB_Max,
		float RPM_Min,
		float RPM_Max,
		float RPS_Min,
		float RPS_Max,
		float MeshSizeB,
		float MeshSizeM,
		float MeshSizeS);

	void DebugMes(int32 Key, FString Message, FColor Color = FColor::Green, float Duration = 2.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Delta")
	float DeltaSeconds;



	protected:

		// User Feedback Visuals
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = " Properties | Interaction ")
		void SetUserFeedbackVisuals(float FeedbackDisplayDuration);

		virtual void SetUserFeedbackVisuals_Implementation(float FeedbackDisplayDuration);


		UFUNCTION()
		void RemoveFeedbackWidget();


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Interaction")
		TSubclassOf<UUserWidget> FeedbackWidgetClass;

		// Referencia al widget activo para poder borrarlo
		UPROPERTY()
		UUserWidget* CurrentActiveWidget;

		// Manejador del tiempo para la destrucción
		FTimerHandle TimerHandle_WidgetDestruction;


};
