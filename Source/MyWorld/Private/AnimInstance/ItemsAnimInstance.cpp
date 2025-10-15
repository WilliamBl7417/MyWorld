#include "AnimInstance/ItemsAnimInstance.h"
#include "Player/WorldPlayer.h"
#include "Kismet/GamePlayStatics.h"
#include "Items/InteractItem.h"
#include "Items/Flower.h"
#include "Items/Rock.h"
#include "Items/Wateringcan.h"
#include "Components/SkeletalMeshComponent.h"


UItemsAnimInstance::UItemsAnimInstance()
	: PlayerDistance(0.0f)
	, OwningActorRef(nullptr)
	, PlayerRef(nullptr)
{
}

void UItemsAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	GetAndSetOwningActor();
	GetAndSetPlayerReference();
}

void UItemsAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwningActorRef)
	{
		GetAndSetOwningActor();
	}

	if (!PlayerRef)
	{
		GetAndSetPlayerReference();
	}

	if (OwningActorRef && PlayerRef)
	{
		CalculatePlayerDistance();
	}
}

void UItemsAnimInstance::GetAndSetOwningActor()
{
	USkeletalMeshComponent* OwningComponent = GetSkelMeshComponent();
	if (OwningComponent)
	{
		OwningActorRef = OwningComponent->GetOwner();
	}
	else
	{

	}
}

void UItemsAnimInstance::GetAndSetPlayerReference()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			// ¡Esta es la clave para castear a AWorldPlayer*!
			PlayerRef = Cast<AWorldPlayer>(PlayerPawn);
		}
	}
	else
	{

	}
}

void UItemsAnimInstance::CalculatePlayerDistance()
{
	if (OwningActorRef && PlayerRef)
	{
		PlayerDistance = OwningActorRef->GetDistanceTo(PlayerRef);
	}
	else
	{
		PlayerDistance = 0.0f;

	}
}