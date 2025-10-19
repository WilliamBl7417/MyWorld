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

	ARock();

	virtual void BeginPlay() override;

	
};
