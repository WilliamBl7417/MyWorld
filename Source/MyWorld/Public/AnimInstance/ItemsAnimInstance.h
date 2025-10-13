// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ItemsAnimInstance.generated.h"

class AWorldPlayer;
class AInteractItem;


/**
 * 
 */
UCLASS()
class MYWORLD_API UItemsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;


};
