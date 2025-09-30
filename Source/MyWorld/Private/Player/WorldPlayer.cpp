#include "Player/WorldPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/TouchingInterface.h"


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

	EyesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyesMesh"));
	EyesMesh->SetupAttachment(GetMesh());
	ULib = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeULib"));
	ULib->SetupAttachment(GetMesh());
	DLib = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EyeDLib"));
	DLib->SetupAttachment(GetMesh());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CapsuleOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleOverlap->SetCollisionResponseToAllChannels(ECR_Ignore); // Ignora todo por defecto
	CapsuleOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap); // Overlap con el canal de Pawn
	CapsuleOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Overlap); // Overlap con WorldDynamic
	CapsuleOverlap->SetGenerateOverlapEvents(true);

	WalkSpeed = 300.f;
	RunSpeed = 600.f;
	CurrentSpeed = 300.f;
	SpeedInterpRate = 5.f;
	LookSensitivity = 1.f;
	bIsRunning = false;

	OverlappingActor = nullptr;




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

void AWorldPlayer::DebugMessage(int32 Key, FString Message, FColor Color, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Key, Duration, Color, Message);
	}
}

void AWorldPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetSpeed = bIsRunning ? RunSpeed : WalkSpeed;

	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, SpeedInterpRate);

	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;

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

		// Add yaw (left/right) input
		AddControllerYawInput(LookAxisVector.X * LookSensitivity);

		// Add pitch (up/down) input
		AddControllerPitchInput(LookAxisVector.Y * LookSensitivity);
	}
}

void AWorldPlayer::InteractEvent(const FInputActionValue& Value)
{
	CallTouchingBP(OverlappingActor);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Touching"));
	}
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

void AWorldPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappingActor == nullptr)
	{
		OverlappingActor = OtherActor;
	}
}

void AWorldPlayer::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingActor = nullptr;

}

void AWorldPlayer::CallTouchingBP(AActor* ActorOverlap)
{
	if (ActorOverlap && ActorOverlap->GetClass()->ImplementsInterface(UTouchingInterface::StaticClass()))
	{
		ITouchingInterface::Execute_TouchingBP(ActorOverlap);
	}

}


