// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/InteractItem.h"
#include "Flower.generated.h"

/**
 * 
 */
UCLASS()
class MYWORLD_API AFlower : public AInteractItem
{
	GENERATED_BODY()

public:
	AFlower();

	virtual void WateringPlantBP_Implementation() override;

	/*Variables*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flower Properties")
	bool bWasWatered = false;
};
