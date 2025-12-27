#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TextManager.generated.h"

class AWorldPlayer;
class AInteractItem;
class AFlower;
class ARock;
class ATree;
class ACandle;


UCLASS()
class MYWORLD_API ATextManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ATextManager();

	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//void UpdateTextManagerDataBP();
	//virtual void UpdateTextManagerDataBP_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlaysoundWhenItemCollectedBP(int32 Totalobject, int32 CurrentObjects);
	virtual void PlaysoundWhenItemCollectedBP_Implementation(int32 Totalobject, int32 CurrentObjects);

	/* Amount of Objects*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 AmountOfRocks = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 AmountOfFlowers = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 AmountOfTrees = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 AmountOfLights = 0;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Text Manager things | References")
	AWorldPlayer* PlayerRef;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
;
	

};
