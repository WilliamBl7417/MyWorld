
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipableComponent.generated.h"
class AWorldPlayer;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYWORLD_API UEquipableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipableComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Wateringcan Logic")
	void HandleAttachment();
	UFUNCTION(BlueprintCallable, Category = "Wateringcan Logic")
	void EquipItem(AInteractItem* ItemToEquip);
	UFUNCTION(BlueprintCallable, Category = "Wateringcan Logic")
	void UnequipItem(AInteractItem* ItemToEquip);
	UFUNCTION(BlueprintCallable, Category = "Wateringcan Logic")
	void EquiptToCandlePedestal(AInteractItem* ItemToEquip);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipable Component")
	AInteractItem* OwnerAsInsteactItem;


	AWorldPlayer* PlayerRef;




protected:
	
	virtual void BeginPlay() override;

	


	

		
};
