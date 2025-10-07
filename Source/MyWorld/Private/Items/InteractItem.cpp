// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InteractItem.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
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

void AInteractItem::SavePlayerRef(AWorldPlayer* PlayerRef)
{
	WorldPlayerRef = PlayerRef;
}

void AInteractItem::CleanPlayerRef()
{
	WorldPlayerRef = nullptr;
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

