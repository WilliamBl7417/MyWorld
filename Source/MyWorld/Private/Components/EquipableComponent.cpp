// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipableComponent.h"
#include "Player/WorldPlayer.h"
#include "Items/InteracTItem.h"
#include "Items/Wateringcan.h"
#include "Components/MovementPlayerAtencionComponent.h"
#include "Components/BoxComponent.h"


UEquipableComponent::UEquipableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEquipableComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UEquipableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UEquipableComponent::HandleAttachment()
{
	AInteractItem* OwnerAsInsteactItem = Cast<AInteractItem>(GetOwner());

	// 1. Validar que el dueño exista y sea AInteractItem
	if (!OwnerAsInsteactItem)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ATTACH FAIL: Owner is NULL. Component no tiene dueño (o no es InteractItem)."));
		}
		return;
	}

	// 2. Validar referencias críticas antes de intentar equipar
	if (!OwnerAsInsteactItem->WorldPlayerRef || !OwnerAsInsteactItem->WorldPlayerRef->GetMesh() || OwnerAsInsteactItem->AttachSocketName.IsNone())
	{
		// Mensaje de error detallado para ver qué referencia falta
		FString DebugMessage = FString::Printf(TEXT("ATTACH FAIL: PlayerRef: %s, Mesh: %s, Socket: %s. Attachment Abortado."),
			OwnerAsInsteactItem->WorldPlayerRef ? TEXT("OK") : TEXT("NULL"),
			(OwnerAsInsteactItem->WorldPlayerRef && OwnerAsInsteactItem->WorldPlayerRef->GetMesh()) ? TEXT("OK") : TEXT("NULL"),
			OwnerAsInsteactItem->AttachSocketName.IsNone() ? TEXT("EMPTY") : TEXT("OK"));

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DebugMessage);
		}
		return;
	}
	

	if (!OwnerAsInsteactItem->bIsInHand) // Lógica de EQUIPAR (si no está en mano)
	{
		UMovementPlayerAtencionComponent* AtentionComponent = OwnerAsInsteactItem->FindComponentByClass<UMovementPlayerAtencionComponent>();

		if (AtentionComponent)
		{
			AtentionComponent->DeactivateVisuals();
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("DEBUG: AtencionComponent Desactivado."));
		}

		OwnerAsInsteactItem->WorldPlayerRef->IsInHand = 0;

		// Reglas de Attach: SnapToTarget para mantener la posición/rotación relativas al socket.
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

		// *** EJECUCIÓN DEL ATTACHMENT ***
		OwnerAsInsteactItem->GetRootComponent()->AttachToComponent(OwnerAsInsteactItem->WorldPlayerRef->GetMesh(), AttachRules, OwnerAsInsteactItem->AttachSocketName);

		// Colisiones (Desactivar)
		if (OwnerAsInsteactItem->StaticMeshComp) OwnerAsInsteactItem->StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (OwnerAsInsteactItem->ShadowMeshComp) OwnerAsInsteactItem->ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (OwnerAsInsteactItem->BoxComp) OwnerAsInsteactItem->BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		OwnerAsInsteactItem->bIsInHand = true;

		if (GEngine)
		{
			// DEBUG: Mensaje de Éxito al Equipar
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SUCCESS: Item %s ATTACHED on socket %s"), *OwnerAsInsteactItem->GetName(), *OwnerAsInsteactItem->AttachSocketName.ToString()));
		}
	}
	else // Lógica de DESEQUIPAR (si ya está en mano)
	{
		OwnerAsInsteactItem->WorldPlayerRef->IsInHand = 50;

		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
		// *** EJECUCIÓN DEL DETACHMENT ***
		OwnerAsInsteactItem->GetRootComponent()->DetachFromComponent(DetachRules);

		// Colisiones (Reactivar)
		if (OwnerAsInsteactItem->StaticMeshComp)OwnerAsInsteactItem->StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		if (OwnerAsInsteactItem->ShadowMeshComp) OwnerAsInsteactItem->ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		if (OwnerAsInsteactItem->BoxComp) OwnerAsInsteactItem->BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		OwnerAsInsteactItem->CleanPlayerRef();

		OwnerAsInsteactItem->bIsInHand = false;
	
	

		if (OwnerAsInsteactItem->FindComponentByClass<UMovementPlayerAtencionComponent>())
		{
			OwnerAsInsteactItem->FindComponentByClass<UMovementPlayerAtencionComponent>()->ActivateVisuals();
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("DEBUG: AtencionComponent Reactivado."));
		}

		if (GEngine)
		{
			// DEBUG: Mensaje de Éxito al Desequipar
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("SUCCESS: Item %s DETACHED (dropped)"), *OwnerAsInsteactItem->GetName()));
		}
	}
}

