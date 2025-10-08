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
class UParticleSystem;
class USoundBase;


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

	UFUNCTION(BlueprintCallable, Category = "Interaction |")
	void SavePlayerRef(AWorldPlayer* PlayerRef);
	UFUNCTION(BlueprintCallable, Category = "Interaction |")
	void CleanPlayerRef();

	/*Internal Functions*/

	virtual void Tick(float DeltaTime) override;

	/*Functions*///

	/*Variables*////
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | StaticMesh")
	UStaticMeshComponent* ShadowMeshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components | BoxCollition")
	UBoxComponent* BoxComp;

	/*Variables*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | BoxCollitions")
	FVector CollitionBoxScale = FVector(1.f, 1.f, .2f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | BoxCollition")
	float CollitionBoxScaleMultiplier = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties |")
	int32 ElementIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Material dynamic")
	UMaterialInterface* RockMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Material dynamic")
	FName MaterialParameterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Material dynamic")
	bool AlreadyChangeMat = false; 

	/*Sonidos y particulas*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Sound and Particles")
	USoundBase* TouchingSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Sound and Particles")
	UParticleSystem* TouchingParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Sound and Particles")
	USoundBase* WateringSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Sound and Particles")
	UParticleSystem* WateringParticles;
	/*Sonidos y particulas*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Interaction")
	bool bImplementTouching = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Interaction")
	bool bImplementWhatering = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Interaction")
	bool bIsInHand = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties | References")
	AWorldPlayer* WorldPlayerRef;
	/*Variables*/

	/*Functions*/
	/*Internal functions*/
	virtual void BeginPlay() override;


protected:

	void DebugMes(int32 Key, FString Message, FColor Color = FColor::Green, float Duration = 2.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Delta")
	float DeltaSeconds;


};
