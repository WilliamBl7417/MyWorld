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

	if (OtherActor->IsA(AInteractItem::StaticClass()))
	{
		AInteractItem* NewInteractItem = Cast<AInteractItem>(OtherActor);

		if (!EquipableObject )
		{
			if (NewInteractItem->FindComponentByClass<UEquipableComponent>())
			{
				EquipableObject = NewInteractItem;
				EquipableObject->SavePlayerRef(this);

				if (!EquipableObject->bIsInHand)
				{
					OverlappingInteractItem = EquipableObject;
				}
			}

		}
		else if (EquipableObject)
		{
			if (NewInteractItem->FindComponentByClass<UEquipableComponent>() && NewInteractItem != EquipableObject)
			{
				EquipableObject = NewInteractItem;
				EquipableObject->SavePlayerRef(this);
				if (!EquipableObject->bIsInHand)
				{
					OverlappingInteractItem = EquipableObject;
				}
			}

		}

		//if (!EquipableObject)
		//{
		//	if (NewInteractItem->FindComponentByClass<UEquipableComponent>())
		//	{
		//		EquipableObject = NewInteractItem;
		//		EquipableObject->SavePlayerRef(this);

		//		if (!EquipableObject->bIsInHand)
		//		{
		//			OverlappingInteractItem = EquipableObject;
		//		}
		//	}

		//}

		//if (NewInteractItem->IsA(AWateringcan::StaticClass()))
		//{
		//	EquipableObject = Cast<AWateringcan>(NewInteractItem);
		//	EquipableObject->SavePlayerRef(this);

		//	if (!EquipableObject->bIsInHand)
		//	{
		//		OverlappingInteractItem = EquipableObject;
		//	}
		//}
		// 
		// 
		//else if (NewInteractItem != ItemInHand)//si es otro objeto
		//{
		//	if (OverlappingInteractItem == nullptr)
		//	{
		//		OverlappingInteractItem = NewInteractItem;
		//		NewInteractItem->SavePlayerRef(this);

		//	}
		//}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Overlap con: %s"), *OtherActor->GetName()));
	}
}

void AWorldPlayer::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	OverlappingActor = nullptr;


	AInteractItem* EndedOverlapItem = Cast<AInteractItem>(OtherActor);

	if (IsValid(EndedOverlapItem))
	{
		if (EndedOverlapItem->IsA(AWateringcan::StaticClass()))
		{
			if (EndedOverlapItem == OverlappingInteractItem)
			{
				OverlappingInteractItem = nullptr;

			}
			return;
		}

		if (EndedOverlapItem == OverlappingInteractItem)
		{
			if (!EndedOverlapItem->bIsInHand)
			{
				//EndedOverlapItem->CleanPlayerRef();
				OverlappingInteractItem = nullptr;
				DebugMessage(-1, FString::Printf(TEXT("End Overlap y limpieza de: %s"), *OtherActor->GetName()), FColor::Yellow, 2.f);
			}
		}
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
	if(IsValid(EquipableObject))
	{
		if (EquipableObject->bIsInHand)//esto hay que ajustarlo para poner la candela en su lugar
		{
			if (OverlappingInteractItem != nullptr && OverlappingInteractItem->bImplementWhatering)
			{
				if (OverlappingInteractItem->IsA(AFlower::StaticClass()))
				{
					AFlower* Flower = Cast<AFlower>(OverlappingInteractItem);
					if (Flower->bWasWatered == false)
					{
						if (EquipableObject->IsA<AWateringcan>())
						{
							AWateringcan* WateringCan = Cast<AWateringcan>(EquipableObject);
							WateringCan->bWasWateredRef = Flower->bWasWatered;
							CallWateringBP(OverlappingInteractItem);
							CallWateringBP(EquipableObject);
							WateringCan->bWasWateredRef = Flower->bWasWatered;
						}
					}
					else
					{
						CallTouchingBP(OverlappingInteractItem);
					}
				}
				return;
			}

			CallTouchingBP(EquipableObject);

			if (!EquipableObject->bIsInHand)
			{
				EquipableObject = nullptr;
			}
			return;
		}

		else if (OverlappingInteractItem == EquipableObject)
		{
			CallTouchingBP(EquipableObject);

			if (EquipableObject->bIsInHand)
			{
				OverlappingInteractItem = nullptr;
			}
			return;
		}
	}

	//if (ItemInHand != nullptr) //
	//{
	//	if (ItemInHand->bImplementTouching)
	//	{
	//		CallTouchingBP(ItemInHand);

	//		if (!ItemInHand->bIsInHand)
	//		{
	//			ItemInHand = nullptr;
	//		}
	//		return;
	//	}
	//}

	//if (IsValid(OverlappingInteractItem))
	//{
	//	if (OverlappingInteractItem->bImplementTouching)
	//	{
	//		CallTouchingBP(OverlappingInteractItem);
	//		if (OverlappingInteractItem->bIsInHand)
	//		{
	//			ItemInHand = OverlappingInteractItem;
	//			OverlappingInteractItem = nullptr;
	//		}
	//		return;
	//	}
	//}
}

void AWorldPlayer::JumpEvent(const FInputActionValue& Value)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Jump Event Triggered"));
	}
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
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MontageToPlay is nullptr"));
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