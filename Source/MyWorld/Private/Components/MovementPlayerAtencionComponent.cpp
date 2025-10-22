// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MovementPlayerAtencionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PointLightComponent.h" 

UMovementPlayerAtencionComponent::UMovementPlayerAtencionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false;
}

UPrimitiveComponent* UMovementPlayerAtencionComponent::FindTargetMesh()
{
    if (!GetOwner()) return nullptr;

    UStaticMeshComponent* StaticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    if (StaticMesh)
    {
        return StaticMesh;
    }

    USkeletalMeshComponent* SkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
    if (SkeletalMesh)
    {
        return SkeletalMesh;
    }

    UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    if (RootPrimitive)
    {
        return RootPrimitive;
    }

    return nullptr;
}


void UMovementPlayerAtencionComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (Owner)
    {
        TargetMesh = FindTargetMesh();
        if (TargetMesh)
        {
            InitialScale = TargetMesh->GetRelativeScale3D();
        }

        InitialRelativeLocation = Owner->GetRootComponent()->GetRelativeLocation();
        InitialRotation = Owner->GetActorRotation();

        TargetLight = Owner->FindComponentByClass<UPointLightComponent>();
        if (TargetLight)
        {
            InitialLightIntensity = TargetLight->Intensity;
            TargetLight->SetIntensity(0.0f);
            TargetLight->SetVisibility(false);
        }
    }
}

void UMovementPlayerAtencionComponent::ActivateVisuals()
{
    SetComponentTickEnabled(true);
    TimeElapsed = 0.0f;

    if (GetOwner() && GetOwner()->GetRootComponent())
    {
        InitialRelativeLocation = GetOwner()->GetActorLocation();
    }
}


void UMovementPlayerAtencionComponent::DeactivateVisuals()
{
    SetComponentTickEnabled(false);

    AActor* Owner = GetOwner();
    if (Owner)
    {
        Owner->SetActorRelativeLocation(InitialRelativeLocation);
        Owner->SetActorRotation(InitialRotation);

        if (TargetMesh)
        {
            TargetMesh->SetRelativeScale3D(InitialScale);
        }

        if (TargetLight)
        {
            TargetLight->SetIntensity(0.0f);
            TargetLight->SetVisibility(false);
        }
    }
}


void UMovementPlayerAtencionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!GetOwner()) return;

    TimeElapsed += DeltaTime * MovementSpeed;

    if (bEnableMovement && MovementType != EVisualMovementType::EVMT_None)
    {
        FVector NewLocation = InitialRelativeLocation;
        float SineValue = FMath::Sin(TimeElapsed);

        if (MovementType == EVisualMovementType::EVMT_Vertical)
        {
            float VerticalOffset = SineValue * VerticalAmplitude;
            NewLocation.Z += VerticalOffset;
        }
        else if (MovementType == EVisualMovementType::EVMT_Circular)
        {
            float CosineValue = FMath::Cos(TimeElapsed);
            NewLocation.X += CosineValue * CircularRadius;
            NewLocation.Y += SineValue * CircularRadius;
        }
        GetOwner()->SetActorLocation(NewLocation);
    }

    if (bEnableRotation)
    {
        FRotator CurrentRotation = GetOwner()->GetActorRotation();
        CurrentRotation.Yaw += RotationSpeed * DeltaTime;
        GetOwner()->SetActorRotation(CurrentRotation);
    }

    if (bEnableSquashAndStretch && TargetMesh && MovementType == EVisualMovementType::EVMT_Vertical)
    {
        float AbsSine = FMath::Abs(FMath::Sin(TimeElapsed));
        float FactorZ = 1.0f + (StretchFactor * AbsSine);
        float FactorXY = 1.0f / FMath::Sqrt(FactorZ);
        FVector NewScale = InitialScale;

        if (FMath::Sin(TimeElapsed) > 0)
        {
            NewScale.Z *= FactorZ;
            NewScale.X *= FactorXY;
            NewScale.Y *= FactorXY;
        }
        else
        {
            NewScale.Z *= 1.0f / FactorZ;
            NewScale.X *= FactorZ;
            NewScale.Y *= FactorZ;
        }

        TargetMesh->SetRelativeScale3D(NewScale);
    }
    else if (TargetMesh && TargetMesh->GetRelativeScale3D() != InitialScale)
    {
        TargetMesh->SetRelativeScale3D(InitialScale);
    }

    if (bEnableLightPulse && TargetLight)
    {
        TargetLight->SetVisibility(true);
        float PulseFactor = (FMath::Cos(TimeElapsed * LightPulseSpeed) * 0.5f) + 0.5f;
        TargetLight->SetIntensity(InitialLightIntensity * PulseFactor);
    }
    else if (TargetLight && TargetLight->Intensity > 0.0f)
    {
        TargetLight->SetIntensity(InitialLightIntensity);
        TargetLight->SetVisibility(TargetLight->Intensity > 0.0f);
    }
}