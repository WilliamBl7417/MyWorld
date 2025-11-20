// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlaySoundAndParticles.generated.h"

class UNiagaraSystem;
class USoundBase;
class UNiagaraComponent; 

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYWORLD_API UPlaySoundAndParticles : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlaySoundAndParticles();

	/*Sonidos y particulas*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Sound and Particles")
	UNiagaraComponent* CurrentNiagaraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Sound and Particles")
	USoundBase* SoundToUse;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties | Sound and Particles")
	FVector SocketLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Sound and Particles")
	FName SocktetNameForParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties | Sound and Particles")
	bool bAdjustParticlesTransform;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Interaction | Sounds N Particles")
	void PlaySoundAndParticles(
		USoundBase* SoundToPlay,
		UNiagaraSystem* ParticlesToPlay,
		AActor* TargetActor,
		FName SocketName,
		bool bAttachToSkeletalMesh,
		bool bAttachToStaticMesh,
		FVector LocationOverride,
		FRotator RotationOverride,
		FVector ScaleOverridee,
		UNiagaraComponent*& OutNiagaraComponent
	);

	UFUNCTION(BlueprintCallable, Category = "Interaction | Sounds N Particles")
	void DestroyCurrentParticles();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};