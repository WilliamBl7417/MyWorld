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


UCLASS()
class MYWORLD_API AInteractItem : public AActor, public ITouchingInterface
{
	GENERATED_BODY()

public:

	/*Functions*///

	AInteractItem();

	/* Interface implementation*/
	virtual void TouchingBP_Implementation() override;

	/*Internal Functions*/

	virtual void Tick(float DeltaTime) override;

	/*Functions*///

	/*Variables*////
	/* Components*/
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Mesh")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Mesh")
	USkeletalMesh* SkeletalMeshToShow;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Mesh")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components | Mesh")
	UStaticMeshComponent* ShadowMeshComp;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components | Mesh")
	//UBoxComponent* BoxComp;/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components | Mesh")
	UStaticMesh* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Delta")
	float DeltaSeconds;
	


protected:

	/*Functions*/
	/*Internal functions*/
	virtual void BeginPlay() override;
	void DebugMes(int32 Key, FString Message, FColor Color = FColor::Green, float Duration = 2.0f);

public:	


};
