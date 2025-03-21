// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_PlayerState.h"

#include "Net/UnrealNetwork.h"

void AGAS_PlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGAS_PlayerState, SelectedCharacterIndex);
	DOREPLIFETIME(AGAS_PlayerState, PlayerIds);
}

void AGAS_PlayerState::OnRep_SelectedCharacterIndex()
{
	UE_LOG(LogTemp, Warning, TEXT("玩家 %s 的SelectedCharacterIndex已复制: %d"), 
		*GetOwner()->GetName(), SelectedCharacterIndex);
}