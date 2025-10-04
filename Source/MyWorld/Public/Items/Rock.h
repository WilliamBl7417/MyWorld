// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/InteractItem.h"
#include "Rock.generated.h"

/**
 * 
 */
UCLASS()
class MYWORLD_API ARock : public AInteractItem
{
	GENERATED_BODY()

public:

	/*Functions*/
	ARock();

	/*Functions*/

	/*Variables*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties")
	FVector CollitionBoxScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties")
	float CollitionBoxScaleMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties")
	int32 ElementIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties | Material dynamic")
	UMaterialInterface* RockMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties | Material dynamic")
	FName MaterialParameterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties | Material dynamic")
	float MaterialParameterValue = 1.0f;

	
};
