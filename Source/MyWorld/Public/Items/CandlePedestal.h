#pragma once

#include "CoreMinimal.h"
#include "Items/InteractItem.h"
#include "CandlePedestal.generated.h"

class ACandle;
class USceneComponent;


UCLASS()
class MYWORLD_API ACandlePedestal : public AInteractItem
{
	GENERATED_BODY()

public:

	ACandlePedestal();

	void TouchingBP_Implementation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pedestal")
	void PlaceCandleBP(ACandle* CandleToPlace);
	virtual void PlaceCandleBP_Implementation(ACandle* CandleToPlace);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pedestal")
	bool bIsCandlePlaced = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pedestal")
	USceneComponent* CandleAttachPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pedestal")
	ACandle* CandleRef;




};