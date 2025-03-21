// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_GameInstance.h"

#include "Kismet/GameplayStatics.h"


UGAS_GameInstance::UGAS_GameInstance()
{
	// 初始化默认的角色索引（例如默认选择第 0 个角色）
	SelectedCharacterIndex = 0;

}

void UGAS_GameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Start"));
	UE_LOG(LogTemp, Warning, TEXT("UGAS_GameInstance::Init on %s"), *UGameplayStatics::GetPlatformName());
}