// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TouchingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTouchingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYWORLD_API ITouchingInterface
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) //Con Funcion en blueprints
		void TouchingBP();  //Con Funcion en blueprints
};
