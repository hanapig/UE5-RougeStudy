// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GAS_GameInstance.generated.h"

/**
 * 自定义 GameInstance 用于存储跨地图的数据，比如角色选择索引
 */
UCLASS()
class UGAS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 构造函数，初始化默认值
	UGAS_GameInstance();


	virtual void Init() override;
	
	// 保存角色选择的索引
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Selection")
	int32 SelectedCharacterIndex;
};
