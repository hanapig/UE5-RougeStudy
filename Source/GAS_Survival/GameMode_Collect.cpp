// Fill out your copyright notice in the Description page of Project Settings.


// 文件：GameMode_Collect.cpp

#include "GameMode_Collect.h"

#include "GAS_GameInstance.h"
#include "GAS_PlayerState.h"
#include "GAS_SurvivalPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"



AGameMode_Collect::AGameMode_Collect()
{
	// 构造函数实现
	bUseSeamlessTravel = true;
	PlayerStateClass = AGAS_PlayerState::StaticClass();
    
}


void AGameMode_Collect::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    bUseSeamlessTravel = true;

    
    UGAS_GameInstance* GI = Cast<UGAS_GameInstance>(GetGameInstance());
    if (GI)
    {
        int32 SelectedIndex = GI->SelectedCharacterIndex;
        // 根据 SelectedIndex 生成对应的角色
    }

    
    UE_LOG(LogTemp, Warning, TEXT("2"));
}


void AGameMode_Collect::HandleSeamlessTravelPlayer(AController*& Controller)
{
    Super::HandleSeamlessTravelPlayer(Controller);
    
    // 获取控制器的玩家状态
    AGAS_SurvivalPlayerController* PlayerController = Cast<AGAS_SurvivalPlayerController>(Controller);
    if (PlayerController)
    {
        AGAS_PlayerState* MyPS = PlayerController->GetPlayerState<AGAS_PlayerState>();
        if (MyPS)
        {
            // 存储索引以便之后在生成角色时使用
            int32 SelectedIndex = MyPS->SelectedCharacterIndex;
            UE_LOG(LogTemp, Warning, TEXT("HandleSeamlessTravelPlayer: %s with SelectedIndex = %d"), 
                *Controller->GetName(), SelectedIndex);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("3"));
}


APawn* AGameMode_Collect::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{

    UGAS_GameInstance* GI = Cast<UGAS_GameInstance>(GetGameInstance());
    int32 SelectedIndex = 0;  // 默认值

    if (GI)
    {
        SelectedIndex = GI->SelectedCharacterIndex;
        UE_LOG(LogTemp, Warning, TEXT("SpawnDefaultPawnFor: SelectedIndex from GameInstance = %d"), SelectedIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnDefaultPawnFor: GameInstance is not UGAS_GameInstance!"));
    }

    
    AGAS_SurvivalPlayerController* PC = Cast<AGAS_SurvivalPlayerController>(NewPlayer);
    if (!PC)
    {
        return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
    }
    
    AGAS_PlayerState* PS = PC->GetPlayerState<AGAS_PlayerState>();
    if (!PS)
    {
        return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
    }
    
    // 获取玩家选择的索引
  
    
    
    // 确保索引在有效范围内
    if (CharacterClasses.IsValidIndex(SelectedIndex) && CharacterClasses[SelectedIndex])
    {
        // 如果是服务端生成角色
        if (HasAuthority())
        {
            // 生成角色并确保服务器拥有控制权
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            
            // 查找合适的出生点（避免重叠）
            AActor* BestStartSpot = FindPlayerStart(NewPlayer, FString::FromInt(PS->PlayerIds));
            FTransform SpawnTransform;
            if (BestStartSpot)
            {
                SpawnTransform = BestStartSpot->GetActorTransform();
            }
            else if (StartSpot)
            {
                SpawnTransform = StartSpot->GetActorTransform();
            }
            
            // 生成角色
            APawn* Character = GetWorld()->SpawnActor<APawn>(
                CharacterClasses[SelectedIndex], 
                SpawnTransform.GetLocation(),
                SpawnTransform.GetRotation().Rotator(),
                SpawnParams
            );
            
            if (Character)
            {
                // 服务器控制角色
                NewPlayer->Possess(Character);
                return Character;
            }
        }
 
    }
    UE_LOG(LogTemp, Warning, TEXT("为玩家ID %d 生成角色，SelectedCharacterIndex = %d"), PS->PlayerIds, PS->SelectedCharacterIndex);

    
    return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}
