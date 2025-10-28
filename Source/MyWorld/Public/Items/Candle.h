// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/InteractItem.h"
#include "Candle.generated.h"

class UMovementPlayerAtencionComponent;
class UEquipableComponent;

/**
 * 
 */
UCLASS()
class MYWORLD_API ACandle : public AInteractItem
{
	GENERATED_BODY()

public:
	ACandle();

	virtual void TouchingBP_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMovementPlayerAtencionComponent* MovementPlayerAtentionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UEquipableComponent* EquipableComponent;
	
};
