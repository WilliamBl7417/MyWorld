// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/TextManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/WorldPlayer.h"

// Sets default values
ATextManager::ATextManager()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}
void ATextManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ATextManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<AWorldPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));

}
