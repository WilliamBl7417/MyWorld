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
#include "Components/PlaySoundAndParticles.h"
#include "Blueprint/UserWidget.h"

UUserWidget* AInteractItem::GlobalActiveWidget = nullptr;
FTimerHandle AInteractItem::GlobalTimerHandle;

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

	ObjectEfects = CreateDefaultSubobject<UPlaySoundAndParticles>(TEXT("ObjectEfects"));

	WorldPlayerRef = nullptr;


}


void AInteractItem::TouchingBP_Implementation()
{
}

void AInteractItem::WateringPlantBP_Implementation()
{
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

void AInteractItem::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractItem::CheckAlreadyChangeMat_Implementation(int32 Valuetochange, EBooleanOutputPin& BranchResult, int32& OutValue)
{
	if (AlreadyChangeMat)
	{
		BranchResult = EBooleanOutputPin::BO_PinTrue;
		OutValue = Valuetochange;
	}
	else
	{
		BranchResult = EBooleanOutputPin::BO_PinFalse;
		if (Valuetochange > 0)
		{
			OutValue = Valuetochange - 1;
		}
		else
		{
			OutValue = Valuetochange;
		}
	}
}

void AInteractItem::SetUserFeedbackVisuals_Implementation(float FeedbackDisplayDuration)
{
	if (!FeedbackWidgetClass) return;

	// 1. Limpieza de seguridad
	if (GlobalActiveWidget)
	{
		GetWorldTimerManager().ClearTimer(GlobalTimerHandle);

		// Antes de remover, quitamos la protección del GC
		GlobalActiveWidget->RemoveFromRoot();
		GlobalActiveWidget->RemoveFromParent();
		GlobalActiveWidget = nullptr;
	}

	// 2. Crear el nuevo widget
	GlobalActiveWidget = CreateWidget<UUserWidget>(GetWorld(), FeedbackWidgetClass);

	if (GlobalActiveWidget)
	{
		// IMPORTANTE: Evita que el GC borre el widget aunque sea estático
		GlobalActiveWidget->AddToRoot();
		GlobalActiveWidget->AddToViewport();

		GetWorldTimerManager().SetTimer(
			GlobalTimerHandle,
			this,
			&AInteractItem::RemoveFeedbackWidget,
			FeedbackDisplayDuration,
			false
		);
	}
}


void AInteractItem::RemoveFeedbackWidget()
{
	if (GlobalActiveWidget)
	{
		GlobalActiveWidget->RemoveFromRoot();
		GlobalActiveWidget->RemoveFromParent();
		GlobalActiveWidget = nullptr;
	}
}

void AInteractItem::DebugMes(int32 Key, FString Message, FColor Color, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Message);
	}
}
void AInteractItem::ChangeMaterialInteraction_Implementation()
{
}
void AInteractItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaSeconds = DeltaTime;
}
