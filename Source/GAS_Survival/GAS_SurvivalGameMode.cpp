#include "GAS_SurvivalGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"

AGAS_SurvivalGameMode::AGAS_SurvivalGameMode()
{
	// 构造函数实现
	bUseSeamlessTravel = true;
}


void AGAS_SurvivalGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 保留玩家控制器，但不在这里自动控制
	TArray<APlayerController*> PlayerControllers;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			PlayerControllers.Add(PC);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("1"));
	
}

void AGAS_SurvivalGameMode::CheckAllPlayersReady()
{
	// 遍历世界里的所有 PlayerController
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AGAS_SurvivalPlayerController* PC = Cast<AGAS_SurvivalPlayerController>(*It);
		if (PC && !PC->bIsReady)
		{
			// 只要有一个玩家没准备，直接返回
			return;
		}
	}
	
	
	GetWorld()->ServerTravel("/Game/Map/Map1?listen");
}