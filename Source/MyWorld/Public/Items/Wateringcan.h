// Wateringcan.h
#pragma once

#include "CoreMinimal.h"
#include "Items/InteractItem.h"
#include "Wateringcan.generated.h"
class UMovementPlayerAtencionComponent;
class UEquipableComponent;

/**
 * */
UCLASS()
class MYWORLD_API AWateringcan : public AInteractItem
{
	GENERATED_BODY()

public:

	AWateringcan();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wateringcan Properties")
	bool bWasWateredRef = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipable Properties")
	UEquipableComponent* EquipableComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Properties")
	UMovementPlayerAtencionComponent* MovementPlayerAtencionComponent;

	virtual void TouchingBP_Implementation() override;

	virtual void WateringPlantBP_Implementation() override;


};