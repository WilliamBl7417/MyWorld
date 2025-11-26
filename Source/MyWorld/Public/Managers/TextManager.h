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

	/* Amount of Objects*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	ARock* AmountOfRocks = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	AFlower* AmountOfFlowers = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	ATree* AmountOfTrees = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References | Amount of Objects")
	int32 AmountOfLights = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Text Manager things | References")
	AWorldPlayer* PlayerRef;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
