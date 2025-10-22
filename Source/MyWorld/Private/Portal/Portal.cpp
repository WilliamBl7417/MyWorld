// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/Portal.h"
#include "Components/PlaySoundAndparticles.h"
#include "Kismet/GameplayStatics.h"
#include "Player/WorldPlayer.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootComp;

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(RootComponent);

	PortalShadowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalShadowMesh"));
	PortalShadowMesh->SetupAttachment(PortalMesh);

	PortalEffects = CreateDefaultSubobject<UPlaySoundAndParticles>(TEXT("PortalEffects"));

	WorldPlayerRef = nullptr;

	EfectsOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("EffectsOverlap"));
	EfectsOverlap->SetupAttachment(RootComponent);

	ChangeLevelOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("ChangeLevelOverlap"));
	ChangeLevelOverlap->SetupAttachment(RootComponent);
}

void APortal::BeginPlay()
{
	Super::BeginPlay();
	WorldPlayerRef = Cast<AWorldPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
	
}

void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



