// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_SurvivalPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GAS_SurvivalCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "GAS_SurvivalGameMode.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AGAS_SurvivalPlayerController::AGAS_SurvivalPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	CurrentIndex = 0;
}

void AGAS_SurvivalPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), FoundActors);

	// 将发现的角色添加到可用角色列表中
	for (AActor* Actor : FoundActors)
	{
		ACharacter* FoundChar = Cast<ACharacter>(Actor);
		if (FoundChar)
		{
			AvailableCharacters.Add(FoundChar);
		}
	}
	


	
}

void AGAS_SurvivalPlayerController::SetupInputComponent()
{

	Super::SetupInputComponent();


	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
	
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AGAS_SurvivalPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AGAS_SurvivalPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AGAS_SurvivalPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AGAS_SurvivalPlayerController::OnSetDestinationReleased);

	
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AGAS_SurvivalPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AGAS_SurvivalPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AGAS_SurvivalPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AGAS_SurvivalPlayerController::OnTouchReleased);
	}

}

void AGAS_SurvivalPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AGAS_SurvivalPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AGAS_SurvivalPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AGAS_SurvivalPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AGAS_SurvivalPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AGAS_SurvivalPlayerController::SwitchCharacterRequest(int32 Direction)
{

	if (!IsLocalController())
	{
		return;
	}
    
	if (HasAuthority()) 
	{
		CurrentIndex = CurrentIndex + Direction;
		if (AvailableCharacters.Num() > 0)
		{
			CurrentIndex = (CurrentIndex % AvailableCharacters.Num() + AvailableCharacters.Num()) % AvailableCharacters.Num();
		}
		SwitchCharacter(CurrentIndex);
	}
	else
	{
		ServerSwitchCharacterRequest(Direction);
	}
}



bool AGAS_SurvivalPlayerController::ServerSwitchCharacterRequest_Validate(int32 Direction)
{
    
    return true;
}

void AGAS_SurvivalPlayerController::ServerSwitchCharacterRequest_Implementation(int32 Direction)
{

	if (HasAuthority() && AvailableCharacters.Num() > 0)
	{
		CurrentIndex = CurrentIndex + Direction;
        
    
		CurrentIndex = (CurrentIndex % AvailableCharacters.Num() + AvailableCharacters.Num()) % AvailableCharacters.Num();

		SwitchCharacter(CurrentIndex);
	}
}



void AGAS_SurvivalPlayerController::SwitchCharacter(int32 NewIndex)
{
	if (!AvailableCharacters.IsValidIndex(NewIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SwitchCharacter: Invalid index %d"), NewIndex);
		return;
	}

	ACharacter* NewChar = AvailableCharacters[NewIndex];
	if (NewChar)
	{
	
		Possess(NewChar);
	}
}


void AGAS_SurvivalPlayerController::OnPossessButtonClicked()
{
 
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), FoundActors);
    
    if (FoundActors.Num() > 0)
    {
        ACharacter* Characters = Cast<ACharacter>(FoundActors[0]); 
        if (Characters)
        {
            Possess(Characters);  
        }
    }
}


void AGAS_SurvivalPlayerController::ServerSetReady_Implementation(bool bNewReady)
{
	// 在服务器端更新准备状态
	bIsReady = bNewReady;

	// 获取GameMode并调用检查函数
	if (AGAS_SurvivalGameMode* GM = Cast<AGAS_SurvivalGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->CheckAllPlayersReady();
	}
}

bool AGAS_SurvivalPlayerController::ServerSetReady_Validate(bool bNewReady)
{
	// 一般简单的RPC可直接返回true
	return true;
}

void AGAS_SurvivalPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 让 bIsReady 在网络中进行复制
	DOREPLIFETIME(AGAS_SurvivalPlayerController, bIsReady);
}

