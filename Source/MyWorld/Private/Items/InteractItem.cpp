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
	WorldPlayerRef = PlayerRef;
}

void AInteractItem::CleanPlayerRef()
{
	WorldPlayerRef = nullptr;
}

void AInteractItem::PlaySoundAndParticlesAtLocation(USoundBase* SoundToPlay, UNiagaraSystem* ParticlesToPlay, FTransform ParticleTransform)
{
	if (SoundToPlay != nullptr && ParticlesToPlay != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Cast<UNiagaraSystem>(ParticlesToPlay), ParticleTransform.GetLocation(), ParticleTransform.GetRotation().Rotator(), ParticleTransform.GetScale3D(), true, true, ENCPoolMethod::AutoRelease, true);
	}
	else if (SoundToPlay != nullptr && ParticlesToPlay == nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
	}
	else if (SoundToPlay == nullptr && ParticlesToPlay != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Cast<UNiagaraSystem>(ParticlesToPlay), ParticleTransform.GetLocation(), ParticleTransform.GetRotation().Rotator(), ParticleTransform.GetScale3D(), true, true, ENCPoolMethod::AutoRelease, true);
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

