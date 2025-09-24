// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TouchingInterface.h"
#include "InteractItem.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComp;






protected:
	/*Functions*/

	/*Internal functions*/
	virtual void BeginPlay() override;

public:	


};
