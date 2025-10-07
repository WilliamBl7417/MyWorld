// Wateringcan.cpp
#include "Items/Wateringcan.h"
#include "Player/WorldPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

AWateringcan::AWateringcan()
{

	if (StaticMeshComp)
	{
		StaticMeshComp->SetMobility(EComponentMobility::Movable);
	}
}

void AWateringcan::TouchingBP_Implementation()
{
	HandleAttachment();
}

void AWateringcan::HandleAttachment()
{
	if (!WorldPlayerRef || !WorldPlayerRef->GetMesh() || AttachSocketName.IsNone())
	{
		DebugMes(-1, TEXT("Attachment failed: Missing Player Mesh or Socket Name."), FColor::Red, 2.0f);
		return;
	}
	// adjunto/desadjuntado
	if (!bIsInHand)
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		StaticMeshComp->AttachToComponent(WorldPlayerRef->GetMesh(), AttachRules, AttachSocketName);

		StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ShadowMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		bIsInHand = true;
		DebugMes(-1, TEXT("Wateringcan ATTACHED!"), FColor::Blue, 2.0f);
	}
	else // Si SÍ está en la mano, lo desadjuntamos
	{
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
		StaticMeshComp->DetachFromComponent(DetachRules);

		StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		CleanPlayerRef();

		bIsInHand = false;
		DebugMes(-1, TEXT("Wateringcan DETACHED!"), FColor::Blue, 2.0f);
	}
};