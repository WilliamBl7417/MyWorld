#include "Components/PlaySoundAndParticles.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"      
#include "Items/InteractItem.h"    
#include "Components/SkeletalMeshComponent.h" 
#include "Components/StaticMeshComponent.h"  
#include "Components/SceneComponent.h"       


UPlaySoundAndParticles::UPlaySoundAndParticles()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlaySoundAndParticles::BeginPlay()
{
	Super::BeginPlay();

}

void UPlaySoundAndParticles::PlaySoundAndParticles(
	USoundBase* SoundToPlay,
	UNiagaraSystem* ParticlesToPlay,
	AActor* TargetActor,
	FName SocketName,
	bool bAttachToSkeletalMesh,
	bool bAttachToStaticMesh,
	FVector LocationOverride,
	FRotator RotationOverride,
	FVector ScaleOverride,
	UNiagaraComponent*& OutNiagaraComponent
)
{
	// Si TargetActor es null, usa el dueño del componente
	if (!TargetActor)
	{
		TargetActor = GetOwner();
		if (!TargetActor) return;
	}

	if (SoundToPlay)
	{
		if (AInteractItem* InteractItem = Cast<AInteractItem>(TargetActor))
		{
			if (InteractItem->ShoulPlaySound) // ← aquí lees el UPROPERTY
			{
				FVector SoundLocation = TargetActor->GetActorLocation();
				UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, SoundLocation);
			}
		}
	}


	// 2. Handle particles
	if (ParticlesToPlay && TargetActor)
	{
		// Eliminar cualquier sistema de partículas anterior antes de spawnear uno nuevo
		if (CurrentNiagaraComponent)
		{
			DestroyCurrentParticles();//valorar si tengo que elimarlo aqui


		}

		bool bAttached = false;
		UNiagaraComponent* NiagaraComp = nullptr;


		USkeletalMeshComponent* TargetSkeletalMesh = nullptr;
		USceneComponent* TargetStaticAttachComp = nullptr;

		// --- a. Get the target components ---

		AInteractItem* OwnerAsInteractItem = Cast<AInteractItem>(GetOwner());

		// Si el TargetActor es el Actor dueño de este componente
		if (TargetActor == GetOwner() && OwnerAsInteractItem) // FIX: Uso de GetOwner() y Cast
		{
			// Acceder a las propiedades públicas de AInteractItem
			TargetSkeletalMesh = OwnerAsInteractItem->SkeletalMeshComp;

			if (OwnerAsInteractItem->StaticMeshAttachPoint)
			{
				TargetStaticAttachComp = OwnerAsInteractItem->StaticMeshAttachPoint;
			}

			else
			{
				TargetStaticAttachComp = OwnerAsInteractItem->StaticMeshComp;
			}
		}
		else // Lógica original para cualquier otro actor (incluyendo el dueño si el cast falla, aunque no debería)
		{

			TargetSkeletalMesh = TargetActor->FindComponentByClass<USkeletalMeshComponent>();
			TargetStaticAttachComp = TargetActor->FindComponentByClass<UStaticMeshComponent>();

			// Si no encuentra StaticMeshComponent, intenta con SceneComponent
			if (!TargetStaticAttachComp)
			{
				TargetStaticAttachComp = TargetActor->FindComponentByClass<USceneComponent>();
			}
		}


		// --- b. Try attaching to Skeletal Mesh ---
		if (bAttachToSkeletalMesh && TargetSkeletalMesh)
		{
			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				ParticlesToPlay,
				TargetSkeletalMesh,
				SocketName,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true, true, ENCPoolMethod::AutoRelease, true
			);
			bAttached = true;
		}

		// --- c. Try attaching to Static Mesh (or custom component) ---
		if (!bAttached && bAttachToStaticMesh && TargetStaticAttachComp)
		{
			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				ParticlesToPlay,
				TargetStaticAttachComp,
				SocketName,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true, true, ENCPoolMethod::AutoRelease, true
			);
			bAttached = true;
		}

		// --- d. Fallback: spawn at location ---
		if (!bAttached)
		{
			// Usar la ubicación del TargetActor como fallback
			FVector SpawnLocation = LocationOverride.IsNearlyZero() ? TargetActor->GetActorLocation() : LocationOverride;
			FRotator SpawnRotation = RotationOverride.IsZero() ? FRotator::ZeroRotator : RotationOverride;
			FVector SpawnScale = ScaleOverride.IsNearlyZero() ? FVector(1.f) : ScaleOverride;

			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ParticlesToPlay,
				SpawnLocation,
				SpawnRotation,
				SpawnScale,
				true, true, ENCPoolMethod::AutoRelease, true
			);
		}

		// 3. Save the reference
		CurrentNiagaraComponent = NiagaraComp;

	}
	else if (GEngine && ParticlesToPlay && !TargetActor)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TargetActor was null, cannot attach."));
	}
}

void UPlaySoundAndParticles::PlayOnlyParticles(
	UNiagaraSystem* ParticlesToPlay,
	USceneComponent* AttachComponent,
	FName SocketName,
	FVector LocationOverride,
	FVector Scale,
	bool bAttach,
	UNiagaraComponent*& OutNiagaraComponent
)
{
	if (!ParticlesToPlay)
	{
		OutNiagaraComponent = nullptr;
		return;
	}

	// Validación de escala: Si es (0,0,0), la ponemos a (1,1,1) para que sea visible
	FVector FinalScale = Scale.IsNearlyZero() ? FVector(1.f) : Scale;

	UNiagaraComponent* NewParticles = nullptr;

	if (bAttach && AttachComponent)
	{
		NewParticles = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ParticlesToPlay,
			AttachComponent,
			SocketName,
			LocationOverride,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true
		);

		// Aplicamos la escala después de atachar
		if (NewParticles)
		{
			NewParticles->SetRelativeScale3D(FinalScale);
		}
	}
	else
	{
		FVector SpawnLocation = AttachComponent ? AttachComponent->GetComponentLocation() : LocationOverride;

		NewParticles = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ParticlesToPlay,
			SpawnLocation,
			FRotator::ZeroRotator,
			FinalScale, // Aquí se aplica directamente en el Spawn
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true
		);
	}

	OutNiagaraComponent = NewParticles;
}
void UPlaySoundAndParticles::DestroyCurrentParticles()
{
	if (CurrentNiagaraComponent)
	{
		// Deactivate works because "NiagaraComponent.h" is included
		CurrentNiagaraComponent->Deactivate();
		CurrentNiagaraComponent = nullptr;
		//DebugMes(-1, FString::Printf(TEXT("Niagara Component Destroyed/Deactivated")), FColor::Red, 2.f);
	}
}

void UPlaySoundAndParticles::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}