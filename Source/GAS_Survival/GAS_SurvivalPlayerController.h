// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "GAS_SurvivalPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AGAS_SurvivalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGAS_SurvivalPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed

public:

	UFUNCTION(BlueprintCallable, Category="Character")
	void SwitchCharacterRequest(int32 Direction);

	// 服务器切换角色（RPC）
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSwitchCharacterRequest(int32 Direction);
	
	// 实际执行切换的函数
	void SwitchCharacter(int32 NewIndex);

	UFUNCTION(BlueprintCallable)
	void OnPossessButtonClicked();

private:
	// 存储所有可供切换的角色
	UPROPERTY()
	TArray<ACharacter*> AvailableCharacters;


	// 获取当前控制的角色在数组中的索引
	int32 CurrentIndex = 0;
	
public:
	// 让这个变量在网络上同步
	UPROPERTY(Replicated)
	bool bIsReady = false;

	// 客户端勾选后，调用此函数通知服务器
	UFUNCTION(Server, Reliable,WithValidation,BlueprintCallable)
	void ServerSetReady(bool bNewReady);
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};


