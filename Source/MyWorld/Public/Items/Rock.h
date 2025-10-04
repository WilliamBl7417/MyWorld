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

	virtual void OnConstruction(const FTransform& Transform) override;
	/*Functions*/

	/*Variables*/

	UPROPERTY()
	FVector StaticMeshScale = FVector(1.f,1.f,1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties")
	float StaticMeshScaleMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties")
	int32 ElementIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties | Material dynamic")
	UMaterialInterface* RockMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties | Material dynamic")
	FName MaterialParameterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties | Material dynamic")
	float MaterialParameterValueMin = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties | Material dynamic")
	float MaterialParameterValueMax = 1.0f;

	
};
