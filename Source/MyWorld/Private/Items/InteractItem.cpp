// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InteractItem.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Player/WorldPlayer.h"

AInteractItem::AInteractItem()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = SceneComponent;

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(RootComponent);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	ShadowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShadowMeshComp"));
	ShadowMeshComp->SetupAttachment(StaticMeshComp);

	StaticMeshAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("StaticMeshAttachPoint")); // <-- ADD THIS
	StaticMeshAttachPoint->SetupAttachment(RootComponent);

	WorldPlayerRef = nullptr;

}


void AInteractItem::TouchingBP_Implementation()
{
	//here i can put some c++ code that will be executed when the blueprint implementation is called
	//also i can leave it empty and just implement the logic in blueprint
	//DebugMes(-1, FString::Printf(TEXT("TouchingBP_Implementation called")), FColor::Green, 2.f);
}

void AInteractItem::WateringPlantBP_Implementation()
{
	//here i can put some c++ code that will be executed when the blueprint implementation is called
	//also i can leave it empty and just implement the logic in blueprint
	//DebugMes(-1, FString::Printf(TEXT("WateringPlantBP_Implementation called")), FColor::Green, 2.f);
}

void AInteractItem::SetAnimRatePlay(FVector WorldScale, 
	float RPB_Min,
	float RPB_Max,
	float RPM_Min, 
	float RPM_Max, 
	float RPS_Min, 
	float RPS_Max,
	float MeshSizeB,
	float MeshSizeM,
	float MeshSizeS
	)
{
	// Tomamos el menor de los tres ejes como referencia
	float MinAxis = FMath::Min3(WorldScale.X, WorldScale.Y, WorldScale.Z);

	if (MinAxis <= MeshSizeS)
	{
		// Tamaño pequeño
		SkeletalMeshComp->GlobalAnimRateScale = FMath::FRandRange(RPS_Min, RPS_Max);
	}
	else if (MinAxis > MeshSizeS && MinAxis <= MeshSizeM)
	{
		// Tamaño medio
		SkeletalMeshComp->GlobalAnimRateScale = FMath::FRandRange(RPM_Min, RPM_Max);
	}
	else if (MinAxis > MeshSizeM && MinAxis <= MeshSizeB)
	{
		// Tamaño grande
		SkeletalMeshComp->GlobalAnimRateScale = FMath::FRandRange(RPB_Min, RPB_Max);
	}

	//if (WorldScale.X == MeshSizeB &&
	//	WorldScale.Y == MeshSizeB &&
	//	WorldScale.Z == MeshSizeB)
	//{
	//	SkeletalMeshComp->GlobalAnimRateScale = (FMath::FRandRange(RPM_Min,RPM_Max));
	//}
	//else if (WorldScale.X == MeshSizeS &&
	//		WorldScale.Y == MeshSizeS &&
	//		WorldScale.Z == MeshSizeS)
	//{
	//	SkeletalMeshComp->GlobalAnimRateScale =(FMath::FRandRange(RPS_Min, RPS_Max));

	//}
	//else if (WorldScale.X == MeshSizeM &&
	//	WorldScale.Y == MeshSizeM &&
	//	WorldScale.Z == MeshSizeM)
	//{
	//	SkeletalMeshComp->GlobalAnimRateScale = (FMath::FRandRange(RPB_Min, RPB_Max));

	//}



}


void AInteractItem::SavePlayerRef(AWorldPlayer* PlayerRef)
{
	//WorldPlayerRef = PlayerRef;
	if (PlayerRef == nullptr)
	{
		WorldPlayerRef = Cast<AWorldPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
	}
	else
	{
		WorldPlayerRef = PlayerRef;
	}
}

void AInteractItem::CleanPlayerRef()
{
	WorldPlayerRef = nullptr;
}

// InteractItem.cpp

void AInteractItem::PlaySoundAndParticles(
	USoundBase* SoundToPlay,
	UNiagaraSystem* ParticlesToPlay,
	AActor* TargetActor,
	FName SocketName,
	bool bAttachToSkeletalMesh,
	bool bAttachToStaticMesh,
	FVector LocationOverride,
	FRotator RotationOverride,
	FVector ScaleOverride
)
{
	// Play sound (sound logic is fine)
	if (SoundToPlay)
	{
		FVector SoundLocation = TargetActor ? TargetActor->GetActorLocation() : GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, SoundLocation);
	}

	// Handle particles
	if (ParticlesToPlay && TargetActor)
	{
		bool bAttached = false;

	
		USkeletalMeshComponent* TargetSkeletalMesh = nullptr;
		USceneComponent* TargetStaticAttachComp = nullptr; 

		// --- 1. Get the target components ---
		if (TargetActor == this)
		{
			TargetSkeletalMesh = SkeletalMeshComp;

			if (StaticMeshAttachPoint)
			{
				TargetStaticAttachComp = StaticMeshAttachPoint;
			}
	
			else
			{
				TargetStaticAttachComp = StaticMeshComp;
			}
		}
		else
		{
		
			TargetSkeletalMesh = TargetActor->FindComponentByClass<USkeletalMeshComponent>();
			TargetStaticAttachComp = TargetActor->FindComponentByClass<UStaticMeshComponent>();
		}


		// --- 2. Try attaching to Skeletal Mesh ---
		if (bAttachToSkeletalMesh && TargetSkeletalMesh)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				ParticlesToPlay,
				TargetSkeletalMesh,
				SocketName,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true, true, ENCPoolMethod::AutoRelease, true
			);
			bAttached = true;
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Niagara attached to SkeletalMesh."));
		}

		// --- 3. Try attaching to Static Mesh (or custom component) ---
		if (!bAttached && bAttachToStaticMesh && TargetStaticAttachComp)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				ParticlesToPlay,
				TargetStaticAttachComp,
				SocketName,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true, true, ENCPoolMethod::AutoRelease, true
			);
			bAttached = true;
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Niagara attached to Static/Custom Component."));
		}

		// --- 4. Fallback: spawn at location ---
		if (!bAttached)
		{
			FVector SpawnLocation = LocationOverride.IsNearlyZero() ? GetActorLocation() : LocationOverride;
			FRotator SpawnRotation = RotationOverride.IsZero() ? FRotator::ZeroRotator : RotationOverride;
			FVector SpawnScale = ScaleOverride.IsNearlyZero() ? FVector(1.f) : ScaleOverride;

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ParticlesToPlay,
				SpawnLocation,
				SpawnRotation,
				SpawnScale,
				true, true, ENCPoolMethod::AutoRelease, true
			);

			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Niagara spawned at location (not attached)"));
		}
	}
	else if (GEngine && ParticlesToPlay && !TargetActor)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TargetActor was null, cannot attach."));
	}
}

void AInteractItem::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractItem::DebugMes(int32 Key, FString Message, FColor Color, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Message);
	}
}
void AInteractItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaSeconds = DeltaTime;
}

