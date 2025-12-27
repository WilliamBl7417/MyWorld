#include "Player/WorldPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/TouchingInterface.h"
#include "Items/Wateringcan.h"
#include "Items/InteractItem.h"
#include "Items/Flower.h"
#include "Items/Candle.h"
#include "Items/CandlePedestal.h"
#include "Components/EquipableComponent.h"


AWorldPlayer::AWorldPlayer()
{

	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);

	CapsuleOverlap = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleOverlap"));
	CapsuleOverlap->SetupAttachment(RootComponent);

	ClothesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClothesMesh"));
	ClothesMesh->SetupAttachment(GetMesh());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CapsuleOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleOverlap->SetCollisionResponseToAllChannels(ECR_Ignore); // Ignora todo por defecto
	CapsuleOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap); // Overlap con el canal de Pawn
	CapsuleOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Overlap); // Overlap con WorldDynamic
	CapsuleOverlap->SetGenerateOverlapEvents(true);

	WalkSpeed = 200.f;
	RunSpeed = 600.f;
	CurrentSpeed = 300.f;
	SpeedInterpRate = 5.f;
	LookSensitivity = 1.f;
	bIsRunning = false;

	OverlappingActor = nullptr;

	OverlappingInteractItem = nullptr;
	ItemInHand = nullptr;

}

void AWorldPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetSpeed = bIsRunning ? RunSpeed : WalkSpeed;

	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, SpeedInterpRate);

	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	DeltaSeconds = DeltaTime;

}

void AWorldPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWorldPlayer::MoveEvent);
		EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWorldPlayer::LookEvent);
		EnhancedInputComp->BindAction(InteractAction, ETriggerEvent::Started, this, &AWorldPlayer::InteractEvent);
		EnhancedInputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWorldPlayer::JumpEvent);
		EnhancedInputComp->BindAction(RunAction, ETriggerEvent::Triggered, this, &AWorldPlayer::RunStart);
		EnhancedInputComp->BindAction(RunAction, ETriggerEvent::Completed, this, &AWorldPlayer::RunStop);

	}
}

void AWorldPlayer::CallTouchingBP(AActor* ActorOverlap)
{
	if (ActorOverlap && ActorOverlap->GetClass()->ImplementsInterface(UTouchingInterface::StaticClass()))
	{
		ITouchingInterface::Execute_TouchingBP(ActorOverlap);
	}
}

void AWorldPlayer::CallWateringBP(AActor* ActorOverlap)
{
	if (ActorOverlap && ActorOverlap->GetClass()->ImplementsInterface(UTouchingInterface::StaticClass()))
	{
		ITouchingInterface::Execute_WateringPlantBP(ActorOverlap);
	}
}

void AWorldPlayer::SetAmountOfElementsToWidget_Implementation(int32 Rocks, int32 Flowers, int32 Trees, int32 Lights)
{
	Rocks = CurrentAmountOfRocks;
	Flowers = CurrentAmountOfFlowers;
	Trees = CurrentAmountOfTrees;
	Lights = CurrentAmountOfLights;
	//SetAmountOfElementsToWidget(Rocks, Flowers, Trees, Lights);

}

void AWorldPlayer::SetAmountOfElementsToPlayer_Implementation(int32 Rocks, int32 Flowers, int32 Trees, int32 Lights)
{
	// 1. Guardamos los valores totales en el jugador
	AmountOfRocks = Rocks;
	AmountOfFlowers = Flowers;
	AmountOfTrees = Trees;
	AmountOfLights = Lights;

	CurrentAmountOfFlowers = Flowers;
	CurrentAmountOfRocks = Rocks;
	CurrentAmountOfTrees = Trees;
	CurrentAmountOfLights = Lights;

	// 2. Llamamos a la función que actualiza el Widget 
	// (Pasamos los totales o los actuales según necesite tu UI)
	SetAmountOfElementsToWidget(AmountOfRocks, AmountOfFlowers, AmountOfTrees, AmountOfLights);

	if (GEngine)
	{
		///GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Player Elements Set: Rocks: %d, Flowers: %d, Trees: %d, Lights: %d"), Rocks, Flowers, Trees, Lights));

	}
}

void AWorldPlayer::UpdateSpawnerDataBP_Implementation()
{
	
}

void AWorldPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
	PlayerCharacter = Cast<ACharacter>(this);


	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	CapsuleOverlap->OnComponentBeginOverlap.AddDynamic(this, &AWorldPlayer::OnBeginOverlap);
	CapsuleOverlap->OnComponentEndOverlap.AddDynamic(this, &AWorldPlayer::OnEndOverlap);

}

void AWorldPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) { return; }

	OverlappingActor = OtherActor;

	if (OverlappingActor->IsA(AInteractItem::StaticClass()))
	{
		AInteractItem* NewInteractItem = Cast<AInteractItem>(OtherActor);

		NewInteractItem->SavePlayerRef(this);

		if (bIsInHand == true && NewInteractItem->FindComponentByClass<UEquipableComponent>())//si ya tengo un objeto en la mano y el que estoy tocando es equipable, no hago nada
		{
			return;
		}
		else if (bIsInHand == true && !NewInteractItem->FindComponentByClass< UEquipableComponent>())//si tengo un objeto en la mano y el que estoy tocando no es equipable, lo asigno como overlapping
		{
			OverlappingInteractItem = NewInteractItem;
		}
		else if (bIsInHand == false && NewInteractItem->FindComponentByClass<UEquipableComponent>())//si no tengo nada en la mano y el objeto es equipable, lo asigno como overlapping
		{
			OverlappingInteractItem = NewInteractItem;
		}
		else if (bIsInHand == false && !NewInteractItem->FindComponentByClass<UEquipableComponent>())//
		{
			OverlappingInteractItem = NewInteractItem;
		}

		if (bIsInHand == false)
		{
			if (NewInteractItem->FindComponentByClass<UEquipableComponent>() && !ItemInHand)
			{
				EquipableObject = NewInteractItem;
			}
			if (NewInteractItem->IsA(ACandle::StaticClass()))
			{
				ACandle* Candle = Cast<ACandle>(NewInteractItem);
				if (Candle && Candle->bIsPlaced)
				{
					EquipableObject = nullptr;
					OverlappingInteractItem = nullptr;
				}
			}
		}
	}
}

void AWorldPlayer::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingActor = nullptr;
	OverlappingInteractItem = nullptr;

	if (!ItemInHand)
	{
		EquipableObject = nullptr;
	}
}

void AWorldPlayer::MoveEvent(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AWorldPlayer::LookEvent(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();

		AddControllerYawInput(LookAxisVector.X * LookSensitivity);

		AddControllerPitchInput(LookAxisVector.Y * LookSensitivity);
	}
}

void AWorldPlayer::InteractEvent(const FInputActionValue& Value)
{
	if (OverlappingInteractItem && EquipableObject && !ItemInHand)//equipo el objeto si no tengo nada en la mano
	{
		if (EquipableObject && EquipableObject->FindComponentByClass<UEquipableComponent>())
		{
			EquipableObject->TouchingBP_Implementation();
			bIsInHand = EquipableObject->bIsInHand;
			UpdateSpawnerDataBP(); 
			//UpdateSpawnerDataBP_Implementation();
		}
	}
	else if (!OverlappingInteractItem && ItemInHand)//suelo el objeto que tengo en la mano si no hay nada con lo que interactuar
	{
		if (EquipableObject && EquipableObject->FindComponentByClass<UEquipableComponent>())
		{
			EquipableObject->TouchingBP_Implementation();
			bIsInHand = EquipableObject->bIsInHand;
			UpdateSpawnerDataBP(); 
			//UpdateSpawnerDataBP_Implementation();
		}
	}
	else if (!EquipableObject && OverlappingInteractItem)//si no tengo nada equipado y hay un objeto con el que interactuar, llamo a su BP
	{
		CallTouchingBP(OverlappingInteractItem);
		UpdateSpawnerDataBP(); 
		//UpdateSpawnerDataBP_Implementation();

	}
	else if (ItemInHand && OverlappingInteractItem)
	{
		if (ItemInHand->IsA(AWateringcan::StaticClass()) && OverlappingInteractItem->IsA(AFlower::StaticClass()))
		{
			if (GEngine)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Watering Plant"));
			}
			CallWateringBP(OverlappingInteractItem);
			CallWateringBP(ItemInHand);
			UpdateSpawnerDataBP(); 
			//UpdateSpawnerDataBP_Implementation();
		}
		else if (ItemInHand->IsA(ACandle::StaticClass()) && OverlappingInteractItem->IsA(ACandlePedestal::StaticClass()))
		{
			ACandle* Candle = Cast<ACandle>(ItemInHand);
			ACandlePedestal* CandlePedestal = Cast<ACandlePedestal>(OverlappingInteractItem);
			if (CandlePedestal && Candle)
			{
				Candle->EquipableComponent->EquiptToCandlePedestal(Candle);
				CandlePedestal->PlaceCandleBP_Implementation(Candle);
				UpdateSpawnerDataBP(); 
				//UpdateSpawnerDataBP_Implementation();
			}
		}
		else if (ItemInHand->IsA(ACandle::StaticClass()) && OverlappingInteractItem->IsA(AInteractItem::StaticClass()))
		{
			CallTouchingBP(OverlappingInteractItem);
			UpdateSpawnerDataBP(); 
			//UpdateSpawnerDataBP_Implementation();
		}
		else if (ItemInHand->IsA(AWateringcan::StaticClass()) && OverlappingInteractItem->IsA(AInteractItem::StaticClass()) && !OverlappingInteractItem->IsA(AFlower::StaticClass()))
		{
			CallTouchingBP(OverlappingInteractItem);
			UpdateSpawnerDataBP(); 
			//UpdateSpawnerDataBP_Implementation();
		}
		
	}
}

void AWorldPlayer::JumpEvent(const FInputActionValue& Value)
{
}

void AWorldPlayer::RunStart(const FInputActionValue& Value)
{
	RunStartBP();
}

void AWorldPlayer::RunStop(const FInputActionValue& Value)
{
	RunStopBP();
}

void AWorldPlayer::RunStartBP_Implementation()
{
	bIsRunning = true;
}

void AWorldPlayer::RunStopBP_Implementation()
{
	bIsRunning = false;
}

void AWorldPlayer::playMontageAnim_Implementation(UAnimMontage* MontageToPlay)
{
	if (MontageToPlay == nullptr)
	{
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MontageToPlay is nullptr"));
		}
		return;
	}
}

void AWorldPlayer::DebugMessage(int32 Key, FString Message, FColor Color, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Message);
	}
}
