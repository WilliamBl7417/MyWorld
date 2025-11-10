// Wateringcan.cpp
#include "Items/Wateringcan.h"
#include "Player/WorldPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/MovementPlayerAtencionComponent.h"
#include "Components/EquipableComponent.h"



AWateringcan::AWateringcan()
{
	if (StaticMeshComp)
	{
		StaticMeshComp->SetMobility(EComponentMobility::Movable);
	}

	EquipableComponent = CreateDefaultSubobject<UEquipableComponent>(TEXT("EquipableComponent"));

	MovementPlayerAtencionComponent = CreateDefaultSubobject<UMovementPlayerAtencionComponent>(TEXT("MovementPlayerAtencionComponent"));

}

void AWateringcan::TouchingBP_Implementation()
{
	EquipableComponent->HandleAttachment();
}

void AWateringcan::WateringPlantBP_Implementation()
{
	Super::WateringPlantBP_Implementation();
}
