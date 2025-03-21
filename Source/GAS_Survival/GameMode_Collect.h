// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_Collect.generated.h"

/**
 * 
 */
UCLASS()
class GAS_SURVIVAL_API AGameMode_Collect : public AGameModeBase
{
	GENERATED_BODY()
public:
	
	AGameMode_Collect();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	int32 NextPlayerId = 0;

	// 这里可以放一个数组或方法，根据 SelectedCharacterIndex 获取对应的 Pawn 类
	UPROPERTY(EditDefaultsOnly, Category="CharacterClasses")
	TArray<TSubclassOf<APawn>> CharacterClasses;

protected:
	// 处理无缝旅行的玩家
	virtual void HandleSeamlessTravelPlayer(AController*& Controller) override;
    
	// 为控制器生成默认Pawn
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;


	
	
};
