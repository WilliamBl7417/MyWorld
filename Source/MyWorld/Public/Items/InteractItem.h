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
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USceneComponent* RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMesh* SkeletalMesh;





protected:
	/*Functions*/

	/*Internal functions*/
	virtual void BeginPlay() override;

public:	


};
