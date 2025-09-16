#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WorldPlayer.generated.h"

UCLASS()
class MYWORLD_API AWorldPlayer : public ACharacter
{
	GENERATED_BODY()

public:

	AWorldPlayer();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
