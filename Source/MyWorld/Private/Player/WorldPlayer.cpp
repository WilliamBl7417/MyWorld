#include "Player/WorldPlayer.h"

AWorldPlayer::AWorldPlayer()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}


void AWorldPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWorldPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWorldPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

