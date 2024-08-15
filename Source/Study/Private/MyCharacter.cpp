// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"



// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USc = CreateDefaultSubobject<USpringArmComponent>("USC");
	USc->SetupAttachment(RootComponent);
	Uc = CreateDefaultSubobject<UCameraComponent>("Uc");
	Uc->SetupAttachment(USc);
	USc->bUsePawnControlRotation = true;
	Uc->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UEventsinterface* com = Cast<UEventsinterface>(GetComponentByClass(UEventsinterface::StaticClass()));
	if (com)
	{
		com->Collisioninterface();
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (APlayerController* play = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = play->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			if (InputSubsystem)
			{
				InputSubsystem->AddMappingContext(Map, 0);
				
			}
		}
	}

	if (Input)
	{
		Input->BindAction(W, ETriggerEvent::Triggered, this, &AMyCharacter::MoveW);
		Input->BindAction(S, ETriggerEvent::Triggered, this, &AMyCharacter::MoveW);
		Input->BindAction(A, ETriggerEvent::Triggered, this, &AMyCharacter::MoveD);
		Input->BindAction(D, ETriggerEvent::Triggered, this, &AMyCharacter::MoveD);
		Input->BindAction(MX, ETriggerEvent::Triggered, this, &AMyCharacter::MoveX);
		Input->BindAction(MY, ETriggerEvent::Triggered, this, &AMyCharacter::MoveY);
		UE_LOG(LogTemp, Warning, TEXT("bindcom"));
	}
}

void AMyCharacter::MoveW(const FInputActionValue& value)
{
	float WValue = value.Get<float>();
	AddMovementInput(GetActorForwardVector(), WValue);
}

//void AMyCharacter::MoveS(const FInputActionValue& value)
//{
//	float WValue = value.Get<float>();
//	AddMovementInput(GetActorForwardVector(), WValue);
//}

void AMyCharacter::MoveD(const FInputActionValue& value)
{
	float WValue = value.Get<float>();
	AddMovementInput(GetActorRightVector(), WValue);
}

//void AMyCharacter::MoveA(const FInputActionValue& value)
//{
//	float WValue = value.Get<float>();
//	AddMovementInput(GetActorRightVector(), WValue);
//}

void AMyCharacter::MoveX(const FInputActionValue& val)
{
	float AxisValue = val.Get<float>();
	if (AxisValue != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(AxisValue);
	}
}

void AMyCharacter::MoveY(const FInputActionValue& val)
{
	float PitchValue = val.Get<float>();
	if (Controller)
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC)
		{
			PC->AddPitchInput(PitchValue);
		}
	}
}



