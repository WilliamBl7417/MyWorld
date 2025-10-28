// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Candle.h"
#include "Components/MovementPlayerAtencionComponent.h"
#include "Components/EquipableComponent.h"


ACandle::ACandle()
{
	PrimaryActorTick.bCanEverTick = false;
	MovementPlayerAtentionComponent = CreateDefaultSubobject<UMovementPlayerAtencionComponent>(TEXT("MovementPlayerAtentionComponent"));
	EquipableComponent = CreateDefaultSubobject<UEquipableComponent>(TEXT("EquipableComponent"));
}
void ACandle::TouchingBP_Implementation()
{
    if (EquipableComponent)
    {
        EquipableComponent->HandleAttachment();
    }
}
