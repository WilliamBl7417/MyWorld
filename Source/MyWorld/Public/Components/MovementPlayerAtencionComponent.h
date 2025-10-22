// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementPlayerAtencionComponent.generated.h"

class UPrimitiveComponent;
class UPointLightComponent;

UENUM(BlueprintType)
enum class EVisualMovementType : uint8
{
    EVMT_None UMETA(DisplayName = "None"),
    EVMT_Vertical UMETA(DisplayName = "Vertical (Ping Pong)"),
    EVMT_Circular UMETA(DisplayName = "Circular (Orbit)")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYWORLD_API UMovementPlayerAtencionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMovementPlayerAtencionComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "0. Control | Activation")
    void ActivateVisuals();

    UFUNCTION(BlueprintCallable, Category = "0. Control | Activation")
    void DeactivateVisuals();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. Control")
    bool bEnableMovement = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0. Control")
    bool bEnableRotation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Movement")
    EVisualMovementType MovementType = EVisualMovementType::EVMT_Vertical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Movement")
    float MovementSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Movement", meta = (EditCondition = "MovementType == EVisualMovementType::EVMT_Vertical", ClampMin = "0.1"))
    float VerticalAmplitude = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1. Movement", meta = (EditCondition = "MovementType == EVisualMovementType::EVMT_Circular", ClampMin = "0.1"))
    float CircularRadius = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2. Rotation")
    float RotationSpeed = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. Squash & Stretch")
    bool bEnableSquashAndStretch = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3. Squash & Stretch", meta = (EditCondition = "bEnableSquashAndStretch", ClampMin = "0.0"))
    float StretchFactor = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Light Pulse")
    bool bEnableLightPulse = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4. Light Pulse", meta = (EditCondition = "bEnableLightPulse"))
    float LightPulseSpeed = 4.0f;


private:
    FVector InitialRelativeLocation;
    FRotator InitialRotation;
    FVector InitialScale;
    float InitialLightIntensity;
    float TimeElapsed = 0.0f;

    UPROPERTY()
    UPrimitiveComponent* TargetMesh;

    UPrimitiveComponent* FindTargetMesh();

    UPROPERTY()
    UPointLightComponent* TargetLight;
};