#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ItemsAnimInstance.generated.h"

class AWorldPlayer;
class AInteractItem;
class AActor; // Incluimos AActor ya que OwningActorRef es de este tipo.


UCLASS()
class MYWORLD_API UItemsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UItemsAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	float PlayerDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	AActor* OwningActorRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	AWorldPlayer* PlayerRef;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	virtual void GetAndSetOwningActor();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	virtual void GetAndSetPlayerReference();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	virtual void CalculatePlayerDistance();
};