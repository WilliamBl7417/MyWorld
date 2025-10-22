// Wateringcan.h
#pragma once

#include "CoreMinimal.h"
#include "Items/InteractItem.h"
#include "Wateringcan.generated.h"
class UMovementPlayerAtencionComponent;

/**
 * */
UCLASS()
class MYWORLD_API AWateringcan : public AInteractItem
{
	GENERATED_BODY()

public:

	AWateringcan();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wateringcan Properties")
	FName AttachSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wateringcan Properties")
	bool bWasWateredRef = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Properties")
	UMovementPlayerAtencionComponent* MovementPlayerAtencionComponent;

protected:

	virtual void TouchingBP_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Wateringcan Logic")
	void HandleAttachment();


};