// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HWPlayerState.h"
#include "Net/UnrealNetwork.h"

AHWPlayerState::AHWPlayerState() : PlayerName(TEXT("None")), CurrentTryCount(0), MaxTryCount(3)
{
	bReplicates = true;
}

void AHWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, CurrentTryCount);
	DOREPLIFETIME(ThisClass, MaxTryCount);
}

FString AHWPlayerState::GetPlayerInfo()
{
	FString PlayerInfo = PlayerName + TEXT("(") + FString::FromInt(CurrentTryCount) + TEXT("/") + FString::FromInt(MaxTryCount) + TEXT(")");


	return PlayerInfo;
}