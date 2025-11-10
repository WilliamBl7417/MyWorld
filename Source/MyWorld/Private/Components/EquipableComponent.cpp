#include "Components/EquipableComponent.h"
#include "Player/WorldPlayer.h"
#include "Items/InteracTItem.h"
#include "Items/Candle.h"
#include "Items/CandlePedestal.h"
#include "Items/Wateringcan.h"
#include "Components/MovementPlayerAtencionComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"


UEquipableComponent::UEquipableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEquipableComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerAsInsteactItem = Cast<AInteractItem>(GetOwner());

	PlayerRef = Cast<AWorldPlayer>(GetOwner());
}

void UEquipableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UEquipableComponent::HandleAttachment()
{
	if (!OwnerAsInsteactItem)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ATTACH FAIL: Owner is NULL. Component no tiene dueno (o no es InteractItem)."));
		}
		return;
	}
	if (!OwnerAsInsteactItem->bIsInHand)
	{
		EquipItem(OwnerAsInsteactItem);
	}
	else
	{
		UnequipItem(OwnerAsInsteactItem);
	}
}

void UEquipableComponent::EquipItem(AInteractItem* ItemToEquip)
{
	if (!ItemToEquip->WorldPlayerRef || !ItemToEquip->WorldPlayerRef->GetMesh() || ItemToEquip->AttachSocketName.IsNone())
	{
		FString DebugMessage = FString::Printf(TEXT("ATTACH FAIL: PlayerRef: %s, Mesh: %s, Socket: %s. Attachment Abortado."),
			ItemToEquip->WorldPlayerRef ? TEXT("OK") : TEXT("NULL"),
			(ItemToEquip->WorldPlayerRef && ItemToEquip->WorldPlayerRef->GetMesh()) ? TEXT("OK") : TEXT("NULL"),
			ItemToEquip->AttachSocketName.IsNone() ? TEXT("EMPTY") : TEXT("OK"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DebugMessage);
		}
		return;
	}

	UMovementPlayerAtencionComponent* AtentionComponent = ItemToEquip->FindComponentByClass<UMovementPlayerAtencionComponent>();

	if (AtentionComponent)
	{
		AtentionComponent->DeactivateVisuals();
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("DEBUG: AtencionComponent Desactivado."));
	}

	ItemToEquip->WorldPlayerRef->IsInHand = 0;
	ItemToEquip->WorldPlayerRef->ItemInHand = ItemToEquip;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

	ItemToEquip->GetRootComponent()->AttachToComponent(ItemToEquip->WorldPlayerRef->GetMesh(), AttachRules, ItemToEquip->AttachSocketName);

	if (ItemToEquip->StaticMeshComp) ItemToEquip->StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (ItemToEquip->ShadowMeshComp) ItemToEquip->ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (ItemToEquip->BoxComp) ItemToEquip->BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ItemToEquip->bIsInHand = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SUCCESS: Item %s ATTACHED on socket %s"), *ItemToEquip->GetName(), *ItemToEquip->AttachSocketName.ToString()));
	}
}

void UEquipableComponent::UnequipItem(AInteractItem* ItemToUnequip)
{
	ItemToUnequip->WorldPlayerRef->IsInHand = 50;
	ItemToUnequip->WorldPlayerRef->ItemInHand = nullptr;

	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);

	ItemToUnequip->GetRootComponent()->DetachFromComponent(DetachRules);

	if (ItemToUnequip->StaticMeshComp) ItemToUnequip->StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (ItemToUnequip->ShadowMeshComp) ItemToUnequip->ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (ItemToUnequip->BoxComp) ItemToUnequip->BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	ItemToUnequip->CleanPlayerRef();

	ItemToUnequip->bIsInHand = false;

	if (ItemToUnequip->FindComponentByClass<UMovementPlayerAtencionComponent>())
	{
		ItemToUnequip->FindComponentByClass<UMovementPlayerAtencionComponent>()->ActivateVisuals();
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("DEBUG: AtencionComponent Reactivado."));
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("SUCCESS: Item %s DETACHED (dropped)"), *ItemToUnequip->GetName()));
	}
}

void UEquipableComponent::EquiptToCandlePedestal(AInteractItem* ItemToEquip)
{
	if (!OwnerAsInsteactItem)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ATTACH FAIL: Owner is NULL. Component no tiene dueno (o no es InteractItem)."));
		}
		return;
	}

	AInteractItem* CandleToPlace = OwnerAsInsteactItem->WorldPlayerRef->ItemInHand;

	if (CandleToPlace && CandleToPlace->IsA(ACandle::StaticClass()))
	{
		AInteractItem* OverlappingInteractActor = Cast<AInteractItem>(OwnerAsInsteactItem->WorldPlayerRef->OverlappingInteractItem);

		if (OverlappingInteractActor && OverlappingInteractActor->IsA(ACandlePedestal::StaticClass()))
		{
			ACandlePedestal* CandlePedestal = Cast<ACandlePedestal>(OverlappingInteractActor);
			if (CandlePedestal->bIsCandlePlaced)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ATTACH FAIL: Candle Pedestal is occupied."));
				}
				return;
			}

			//CandleToPlace->bIsPlaced = true;
			UnequipItem(CandleToPlace);

			//CandlePedestal->bIsCandlePlaced = true;

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SUCCESS: Item %s ATTACHED to Candle Pedestal"), *CandleToPlace->GetName()));
			}
			return;
		}
	}
}


//	if (!OwnerAsInsteactItem)
//	{
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ATTACH FAIL: Owner is NULL. Component no tiene dueno (o no es InteractItem)."));
//		}
//		return;
//	}
//
//	AInteractItem* CandleToPlace = OwnerAsInsteactItem->WorldPlayerRef->ItemInHand;
//
//	if (CandleToPlace && CandleToPlace->IsA(ACandle::StaticClass()))
//	{
//		AInteractItem* OverlappingInteractActor = Cast<AInteractItem>(OwnerAsInsteactItem->WorldPlayerRef->OverlappingInteractItem);
//
//		if (OverlappingInteractActor && OverlappingInteractActor->IsA(ACandlePedestal::StaticClass()))
//		{
//			ACandlePedestal* CandlePedestal = Cast<ACandlePedestal>(OverlappingInteractActor);
//			if (CandlePedestal->bIsCandlePlaced)
//			{
//				if (GEngine)
//				{
//					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ATTACH FAIL: Candle Pedestal is occupied."));
//				}
//				return;
//			}
//
//			FTransform TargetWorldTransform = CandlePedestal->CandleAttachPoint->GetComponentTransform();
//
//			UnequipItem(CandleToPlace);
//
//			FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
//
//			CandleToPlace->GetRootComponent()->AttachToComponent(CandlePedestal->CandleAttachPoint, AttachRules);
//
//			// CORRECCIÓN FINAL: SetActorTransform mueve el Actor y usamos TeleportPhysics para asegurar que el movimiento sea limpio y sin simulación.
//			CandleToPlace->SetActorTransform(TargetWorldTransform, false, nullptr, ETeleportType::TeleportPhysics);
//
//			CandlePedestal->bIsCandlePlaced = true;
//
//			if (GEngine)
//			{
//				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SUCCESS: Item %s ATTACHED to Candle Pedestal"), *CandleToPlace->GetName()));
//			}
//			return;
//		}
//	}
//}