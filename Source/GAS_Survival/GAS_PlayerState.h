// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GAS_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GAS_SURVIVAL_API AGAS_PlayerState : public APlayerState
{
	GENERATED_BODY()


public:
	// 选中的角色索引，准备跨地图使用
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="CharacterSelection")
	int32 SelectedCharacterIndex;

	UPROPERTY(Replicated)
	int32 PlayerIds;

	UFUNCTION()
	void OnRep_SelectedCharacterIndex();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
