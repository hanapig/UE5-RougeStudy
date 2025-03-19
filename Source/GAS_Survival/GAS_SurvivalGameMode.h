// GAS_SurvivalGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GAS_SurvivalPlayerController.h"

#include "GAS_SurvivalGameMode.generated.h"

UCLASS()
class GAS_SURVIVAL_API AGAS_SurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGAS_SurvivalGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;


	UFUNCTION(BlueprintCallable)
	void CheckAllPlayersReady();
};
