// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class UPlaySoundAndParticles;
class AWorldPlayer;
class USphereComponent;
class UBoxComponent;

UCLASS()
class MYWORLD_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	USceneComponent* RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UStaticMeshComponent* PortalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UStaticMeshComponent* PortalShadowMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UStaticMesh* MeshAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UPlaySoundAndParticles* PortalEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	USphereComponent* EfectsOverlap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	UBoxComponent* ChangeLevelOverlap;


	/*References*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties | References")
	AWorldPlayer* WorldPlayerRef;

	/*Variables*/



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
