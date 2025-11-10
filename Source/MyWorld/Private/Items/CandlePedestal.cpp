// CandlePedestal.cpp

#include "Items/CandlePedestal.h"
#include "Player/WorldPlayer.h"
#include "Items/Candle.h" 
#include "Components/EquipableComponent.h" 
#include "Components/BoxComponent.h" 
#include "Components/StaticMeshComponent.h" 
#include "Components/SceneComponent.h"
#include "Engine/EngineTypes.h" 
#include "Items/Candle.h"

ACandlePedestal::ACandlePedestal()
{
	CandleAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CandleAttachPoint"));
	CandleAttachPoint->SetupAttachment(RootComponent);

	CandleRef = nullptr;
}

void ACandlePedestal::TouchingBP_Implementation()
{
	Super::TouchingBP_Implementation();
    
}

void ACandlePedestal::PlaceCandleBP_Implementation(ACandle* CandleToPlace)
{
	PlaceCandleBP(CandleToPlace);
}


