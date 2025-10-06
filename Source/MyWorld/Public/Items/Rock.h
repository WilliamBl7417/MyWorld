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

	virtual void BeginPlay() override;

	/*Functions*/

	/*Variables*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties")
	FVector CollitionBoxScale = FVector(1.f,1.f,.2f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties")
	float CollitionBoxScaleMultiplier = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties")
	int32 ElementIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties | Material dynamic")
	UMaterialInterface* RockMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rock Properties | Material dynamic")
	FName MaterialParameterName;

	
};
